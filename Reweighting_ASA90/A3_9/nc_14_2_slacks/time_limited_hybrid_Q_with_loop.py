from dwave.system import LeapHybridSampler
import numpy as np

for i in range(50):
    sampler = LeapHybridSampler()
    lim=3
    h = np.loadtxt('h.txt')
    J = np.loadtxt('J.txt')
    outputname = "Outputs_tlim_"+str(lim)+".txt"
    results = sampler.sample_ising(h,J,time_limit=lim)
    with open(outputname, "a") as out:
        out.write("Time limit: %f\n" % (lim))
        out.write(str(results.info)+"\n")
        out.write(str(list(results.record[0][0]))+"\t"+str(round(results.record[0][1], 5))+"\n")
    print("Time limit: ",lim)
    print(results.info)
    print("Energy = ", results.record[0][1])
    solution = np.array(list(results.record[0][0]))
    print("Python energy= ", round(h @ solution + solution.T @ J @ solution, 5))