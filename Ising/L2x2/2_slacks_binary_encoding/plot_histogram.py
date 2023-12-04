import numpy as np
import matplotlib.pyplot as plt

set_configurations_spins = ['0110','1001','0100','1010','0011','0001','1110','1000','1101','0010','1011','0101','1100','0111','1111','0000']
set_configurations_pairs = ['0000', '0101', '0110', '1001', '1100', '0011', '1010', '1111']

data = open("configurations_loop.txt", "r")
configurations = []
configurations_pairs = []
configurations_spins = []
for row in data:
    row = row.replace("\n", "")
    configurations.append(row)
    configurations_spins.append(row[:4])
    configurations_pairs.append(row[4:8])


mapped_configurations_pairs = []
for i in range(len(configurations_pairs)):
    mapped_configurations_pairs.append(set_configurations_pairs.index(configurations_pairs[i]))

bins = [-0.5+i for i in range(max(mapped_configurations_pairs)+2)]
ticks = [i for i in range(max(mapped_configurations_pairs)+1)]

plt.figure()
counts, bins, fig = plt.hist(mapped_configurations_pairs, bins=bins)
print(counts)
plt.errorbar(ticks, counts, yerr=np.sqrt(counts), fmt='none', color="tab:orange")
plt.plot([bins[0], bins[-1]], [np.sum(counts)/len(ticks), np.sum(counts)/len(ticks)], '--', color='k')

plt.xticks(ticks, [str(t+1) for t in ticks])
plt.xlabel("microstates")
plt.ylabel("N")
plt.title("Histogram of pairs distributions")
plt.savefig('Histogram_uniformity_pairs.png',dpi=300, bbox_inches = "tight")

mapped_configurations_spins = []
for i in range(len(configurations_spins)):
    mapped_configurations_spins.append(set_configurations_spins.index(configurations_spins[i]))

bins = [-0.5+i for i in range(max(mapped_configurations_spins)+2)]
ticks = [i for i in range(max(mapped_configurations_spins)+1)]

plt.figure()
counts, bins, fig = plt.hist(mapped_configurations_spins, bins=bins)
print(counts)
plt.errorbar(ticks, counts, yerr=np.sqrt(counts), fmt='none', color="tab:orange")
plt.plot([bins[0], bins[-1]], [np.sum(counts)/len(ticks), np.sum(counts)/len(ticks)], '--', color='k')

plt.xticks(ticks, [str(t+1) for t in ticks])
plt.xlabel("microstates")
plt.ylabel("N")
plt.title("Histogram of spins distributions")
plt.savefig('Histogram_uniformity_spins.png',dpi=300, bbox_inches = "tight")