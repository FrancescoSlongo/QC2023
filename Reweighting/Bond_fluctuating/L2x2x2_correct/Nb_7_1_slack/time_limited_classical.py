import neal
import numpy as np

h = np.loadtxt('h.txt')
J = np.loadtxt('J.txt')

sampler = neal.SimulatedAnnealingSampler()

lim=3.0
outputname = "Outputs_tlim_"+str(lim)+".txt"
out = open(outputname, 'a')

results = sampler.sample_ising(h,J,num_reads=1,time_limit=lim)
print("Time limit: ",lim)
print(results.info)
out.write("Time limit: %f\n" % (lim))
out.write(str(results.info)+"\n")
out.write(str(list(results.record[0][0]))+"\t"+str(results.record[0][1])+"\n")
print("Energy = ", results.record[0][1])
out.close()
