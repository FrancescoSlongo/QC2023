from dwave.system import DWaveSampler, EmbeddingComposite
import numpy as np

h = np.loadtxt('h.txt')
J = np.loadtxt('J.txt')

sampler = EmbeddingComposite(DWaveSampler(solver={'qpu': True}))

outputname = "Outputs_2000_loop.txt"
out = open(outputname, 'a')
for i in range(10000):
    results = sampler.sample_ising(h,J, annealing_time=2000)
    solution = np.array(list(results.record[0][0]))
    print("Energy = ", results.record[0][1])
    print("Python energy = ", round(h @ solution + solution.T @ J @ solution, 5))
    print(results.info)
    out.write(str(results.info)+"\n")
    out.write(str(list(results.record[0][0]))+"\t"+str(results.record[0][1])+"\n")
    
out.close()
