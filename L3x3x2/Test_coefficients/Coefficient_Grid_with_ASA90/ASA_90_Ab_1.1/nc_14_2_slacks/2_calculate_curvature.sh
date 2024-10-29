#!/bin/bash

# Check if the file is provided as an argument
if [ -z "$1" ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

# Read the file line by line and echo each line
rm -f slacks.txt
touch slacks.txt
while IFS= read -r line; do
  echo ${line: -2}>>slacks.txt
done < "$1"
