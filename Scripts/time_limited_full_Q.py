from dwave.system import DWaveSampler, EmbeddingComposite
import numpy as np

h = np.loadtxt('h.txt')
J = np.loadtxt('J.txt')

sampler = EmbeddingComposite(DWaveSampler(solver={'qpu': True}))

lim=0
outputname = "Outputs_2000.txt"
out = open(outputname, 'a')

results = sampler.sample_ising(h,J, annealing_time=2000)

print("Energy = ", results.record[0][1])
print("Time limit: ",lim)
print(results.info)
out.write("Time limit: %f\n" % (lim))
out.write(str(results.info)+"\n")
out.write(str(list(results.record[0][0]))+"\t"+str(results.record[0][1])+"\n")

out.close()
