# jtc
JSON test console 

Simple offline cli tool to manipulate JSON data format.

jtc features following:
  - simple user interface allowing applying bulk changes in one command
  - featured walk interface let extracting any combination of data from source JSON
  - extracted data is representable either as it found, or as a complete JSON format
  - support Regular Expressions while searching source JSON
  - fast and efficient processing very large JSON files
  - written entirely in C++14, no dependencies
  - extensively debuggable
  - conforms JSON specification (http://json.org/index.html)


Compile instructions:

Linux and MacOS precompiled binaries are available for download

For compiling c++14 is required (or later):
  - to compile under MacOS, use cli: `c++ -o jtc -Wall -std=c++14 -Ofast jtc.cpp`
  - To compile under Linux, use cli: `c++ -o jtc -Wall -std=gnu++14 -static -Ofast jtc.cpp`
  
