from dwave.system import LeapHybridSampler
import numpy as np

def flip_random_spins(h, J, p=0.5):
    N = len(h)
    indices = []
    for i in range(N):
        if np.random.rand(1) < p:
                indices.append(i)
        
    hout = np.copy(h)
    Jout = np.copy(J)
    for idx in indices:
        hout[idx] *= -1
        Jout[:,idx] *= -1
        Jout[idx,:] *= -1
    return hout, Jout, indices
    
for i in range(25):
    print("Iteration = ", i)
    hi = np.loadtxt('h.txt')
    Ji = np.loadtxt('J.txt')

    sampler = LeapHybridSampler()

    lim=3.0
    outputname = "Outputs_tlim_"+str(lim)+"_with_spin_flip.txt"
    out = open(outputname, 'a')

    h, J, indices = flip_random_spins(hi, Ji)
    print(indices)

    results = sampler.sample_ising(h,J,time_limit=lim)

    solution_flipped = np.array(list(results.record[0][0]))
    solution = np.copy(solution_flipped)
    for idx in indices:
        solution[idx] *= -1
    print("Time limit: ",lim)
    print(results.info)
    out.write("Time limit: %f\n" % (lim))
    out.write(str(results.info)+"\n")
    out.write(str(list(solution))+"\t"+str(round(results.record[0][1], 5))+"\n")
    print("Energy = ", results.record[0][1])

    print("Python energy (flipped)= ", round(h @ solution_flipped + solution_flipped.T @ J @ solution_flipped, 5))
    print("Python energy (without Gauge)= ", round(hi @ solution + solution.T @ Ji @ solution, 5))
    out.close()
