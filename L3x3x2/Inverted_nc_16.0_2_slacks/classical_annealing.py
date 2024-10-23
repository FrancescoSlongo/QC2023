# hybrid solver test with 10 edges, 4 vertices
import dimod, neal
import numpy as np

h = np.loadtxt('h.txt')
J = np.loadtxt('J.txt')

sampler = neal.SimulatedAnnealingSampler()
E=0
while E > -344.250000:
	results = sampler.sample_ising(h,J, num_reads=1)
	x=(results.record[0][0]+1)/2
	E=results.record[0][1]
	print(x)
	print(np.sum(x[:33]), np.sum(x[33:-2]))
out = open('Outputs.txt', 'w')

for campione in results.record:
	for Nple in campione:
		print(Nple)
		out.write(str(Nple)+"\t")
	out.write("\n")
out.close()