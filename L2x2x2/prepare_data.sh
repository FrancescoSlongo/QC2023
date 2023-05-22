grep '\[' Outputs_tlim_3.0.txt | sed 's/, //g' | awk '{print $1}' | sed 's/-1/0/g' | sed 's/\[//g' | sed 's/\]//g' > configurations.txt
sort configurations.txt | uniq -c > degeneracies.txt
