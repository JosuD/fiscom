
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = 'pmax.txt'

import os
os.system('rm '+ archivo)

os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)

import numpy as np
from matplotlib import pyplot as plt

Data = np.genfromtxt(archivo, delimiter = '\t')

print('Encontre N = ', np.shape(Data)[0], ' valores, cuyo valor medio es ', np.mean(Data), ' y su desviacion ', np.std(Data))


plt.figure()
x = np.arange(0,len(Data))
plt.plot(x, Data, 'g*')
#plt.xscale('log')
#plt.yscale('log')
plt.title('pmax vs s')


plt.show()

