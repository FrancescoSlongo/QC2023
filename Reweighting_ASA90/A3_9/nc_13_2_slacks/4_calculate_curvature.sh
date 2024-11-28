#!/bin/bash
# Read the file line by line and echo each line
rm -f slacks_6.txt
touch slacks_6.txt
while IFS= read -r line; do
  echo ${line: -2}>>slacks_6.txt
done < "configurations_6.txt"
