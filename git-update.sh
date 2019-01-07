#! /bin/bash

MSG=$1

git add *
git diff --staged
git commit -m "$MSG"
