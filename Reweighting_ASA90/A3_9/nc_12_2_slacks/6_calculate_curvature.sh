#!/bin/bash
# Read the file line by line and echo each line
rm -f slacks_9.txt
touch slacks_9.txt
while IFS= read -r line; do
  echo ${line: -2}>>slacks_9.txt
done < "configurations_9.txt"
