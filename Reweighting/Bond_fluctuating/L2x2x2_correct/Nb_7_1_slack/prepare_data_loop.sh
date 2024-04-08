grep "]	-29.5" Outputs_2000_loop.txt | sed 's/, //g' | awk '{print $1}' | sed 's/-1/0/g' | sed 's/\[//g' | sed 's/\]//g' > configurations_loop.txt
sort configurations_loop.txt | uniq -c > degeneracies_loop.txt
 