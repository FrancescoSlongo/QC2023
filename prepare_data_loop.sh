grep "]	-31.0" Outputs_2000_loop.txt | sed 's/, //g' | awk '{print $1}' | sed 's/-1/0/g' | sed 's/\[//g' | sed 's/\]//g' > configurations_loop.txt
sort configurations_loop.txt | uniq -c > degeneracies_loop.txt
python plot_histogram.py
