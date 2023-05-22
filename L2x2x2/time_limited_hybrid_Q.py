from dwave.system import LeapHybridSampler
import numpy as np

for i in range(13):
    print("Iteration = ", i)
    h = np.loadtxt('h.txt')
    J = np.loadtxt('J.txt')

    sampler = LeapHybridSampler()

    lim=3.0
    outputname = "Outputs_tlim_"+str(lim)+".txt"
    out = open(outputname, 'a')

    results = sampler.sample_ising(h,J,time_limit=lim)
    print("Time limit: ",lim)
    print(results.info)
    out.write("Time limit: %f\n" % (lim))
    out.write(str(results.info)+"\n")
    out.write(str(list(results.record[0][0]))+"\t"+str(round(results.record[0][1], 5))+"\n")
    print("Energy = ", results.record[0][1])
    state = np.array(list(results.record[0][0]))
    print("Python energy = ", round(h@state + state.T@J@state, 5))
    out.close()
