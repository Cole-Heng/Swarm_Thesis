import matplotlib.pyplot as plt
import numpy as np

heatFile = open("sim_statistics.log")
heatArray = np.loadtxt(heatFile, delimiter=",", skiprows=8)

plt.imshow(heatArray, cmap='hot', interpolation='none')
plt.show()
