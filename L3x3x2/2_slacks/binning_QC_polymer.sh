#!/bin/bash
HomeDir=$(pwd)
FILE="slacks.txt"

rm -f INPUT_histograms.dat
touch INPUT_histograms.dat
touch tmp.txt
counter1=1
counter2=0
for nc in {12..15}; do
    folder="nc_${nc}.0_2_slack"
    echo $folder
    cd $folder
    
    if [ -f $FILE ]; then
        echo "File $FILE exists."
        counter1=$((counter1+1))
        sort $FILE | uniq -c | awk '{print $1}' >> a.txt
        counter3=$((counter2+3))
        input_string="${counter2}   ${counter3}"
        while IFS= read -r line
        do
            input_string="${input_string}    ${line}" 
        done < "a.txt"
        input_string="${input_string}    1.0"
        rm -f a.txt
        echo ${input_string} >> ${HomeDir}/tmp.txt
        counter2=$((counter2+1))
    else
        echo "File $FILE does not exist."
    fi
    cd ${HomeDir}
done

counter1=$((counter1+2))
input_string="${counter1}"
for (( c=0; c<$counter1; c++ ))
do
    input_string="${input_string}   ${c}"
done
echo ${input_string} >> INPUT_histograms.dat
cat tmp.txt >> INPUT_histograms.dat
rm -f tmp.txt