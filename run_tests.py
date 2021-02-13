#!/usr/bin/python3
from dataclasses import dataclass
from typing import List
import subprocess
import os
import unittest


ENV = os.environ.copy()
ENV["PATH"] = f".:{ENV['PATH']}"


@dataclass
class CodeBlock(unittest.TestCase):
    command: str
    output: str

    maxDiff = None

    def check(self, verbosity: int = 0, valgrind: bool = False) -> None:
        cmd = self.command if not valgrind else "valgrind -q " + self.command
        if verbosity > 0:
            print(f"command: {cmd}")
        if verbosity > 1:
            print(f"output: {self.output}")

        if self.command.find("=") != -1 and len(self.command.split("=")) == 2:
            var, val = self.command.split("=")
            ENV[var] = val.strip("'")
            return

        proc = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            shell=True,
            env=ENV,
            executable="/usr/bin/bash",
        )
        stdout, stderr = proc.communicate()
        self.assertMultiLineEqual(str(stdout, encoding="utf-8"), self.output)

    def __str__(self):
        return f"CodeBlock: cmd='{self.command}'"


def extract_markdown_code_blocks(fname: str) -> List[List[str]]:
    block_started: bool = False
    block_lines: List[str] = []
    blocks: List[List[str]] = []

    with open(fname, "r") as md_file:
        for line in md_file:
            block_border = line[0:3] == "```"

            if block_border:
                if block_started:
                    block_started = False
                    blocks.append(block_lines)
                    block_lines = []
                else:
                    if (
                        line[3:].find("bash") != -1
                        and line[3:].find("SKIP") == -1
                    ):
                        block_started = True

            if block_started and not block_border:
                block_lines.append(line)

    return blocks


def extract_code_blocks(lines: List[str]) -> List[CodeBlock]:
    output: List[str] = []
    command: str = ""
    code_blocks = []

    cmd_on_line: bool = False
    cmd_pushed: bool = False

    for i, line in enumerate(lines):
        cmd_on_line = line[0:6] == "bash $"

        if cmd_on_line:
            # if command == "" and line[6:] != "":
            #     command = line[6:]
            #     command = command.strip()

            if command != "":
                code_blocks.append(CodeBlock(command, "".join(output)))
                cmd_pushed = True

            output = []
            command = line[6:]
            command = command.strip()
            cmd_pushed = False

        elif line[0] != "#":
            output.append(line)
            # special handling for lines that just set an environment variable
            # e.g.:
            # bash $ jsn="foo"
            # bash $ <<<$jsn jtc
            # here the first line has no output, but must be added nevertheless
            # if output or (lines[i-1][0:6] == "bash $" and i > 0):

    if not cmd_pushed:
        code_blocks.append(CodeBlock(command, "".join(output)))

    return (block for block in code_blocks if block.command != "")


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="jtc test runner")
    parser.add_argument("md_files", nargs="+", type=str)
    parser.add_argument("-v", "--verbose", action="count", default=0)
    parser.add_argument("--valgrind", action="store_true")
    args = parser.parse_args()

    for md_file in args.md_files:
        for blocks in extract_markdown_code_blocks(md_file):
            # print(blocks)
            for block in extract_code_blocks(blocks):
                # print(block)
                block.check(verbosity=args.verbose, valgrind=args.valgrind)
