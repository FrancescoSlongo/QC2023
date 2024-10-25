# hybrid solver test with 10 edges, 4 vertices
import dimod, neal
import numpy as np
import itertools
import collections 

h = np.loadtxt('h.txt')
J = np.loadtxt('J.txt')

sampler = neal.SimulatedAnnealingSampler()
samples=[]
for j in range(100):
	print(j)
	iteration=[]
	for i in range(50):
		E=0
		while (E+416.25)>1e-3:
			results = sampler.sample_ising(h,J, num_reads=1)
			E=results.record[0][1]
		x=(results.record[0][0]+1)/2
		iteration.append(np.sum(x[33:-2]))
		
	counter = dict(collections.Counter(iteration))
	keys= list(counter.keys())
	keys.sort()
	data=[]
	for key in [16, 15, 14, 13]:
		if key in keys:
			data.append(counter[key])
		else:
			data.append(0)
	samples.append(data)
	
samples=np.array(samples)
mean=np.mean(samples, axis=0)
std=np.std(samples, axis=0)
print(mean, std)
with open('Outputs.txt', 'w') as f:
	for i in range(4):
		f.write(f"{mean[i]} \pm {std[i]}\n")
#out = open('Outputs.txt', 'w')

#for campione in results.record:
#	for Nple in campione:
#		print(Nple)
#		out.write(str(Nple)+"\t")
#	out.write("\n")
#out.close()