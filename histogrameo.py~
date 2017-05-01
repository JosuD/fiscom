
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = 'histograma.txt'
archivo2 = 'intensidades.txt'
archivo3 = 'tamanios.txt'

import os
os.system('rm '+ archivo)
os.system('rm '+ archivo2)
os.system('rm '+ archivo3)

os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)


import numpy as np
from matplotlib import pyplot as plt
plt.close()

Data = np.genfromtxt(archivo, delimiter = '\t')
Data2 = np.genfromtxt(archivo2, delimiter = '\t')
Data3 = np.genfromtxt(archivo3, delimiter = '\t')

print('Encontre N = ', np.shape(Data)[0], ' valores, cuyo valor medio es ', np.mean(Data), ' y su desviacion ', np.std(Data))

bines = np.linspace(0,1,len(Data))
y = np.zeros(len(Data))+0.5
#n, bins, patches = plt.hist(Data, bins=bines, facecolor='green', alpha=0.75)
plt.plot(bines, Data, 'r.', bines, y, 'k-')
plt.grid()
plt.title('Funcion cumulativa')

plt.figure()
plt.plot(bines, Data2, 'r.')
plt.grid()
plt.title('Intensidad del cluster')


plt.figure()
x = np.arange(0,len(Data3))
plt.plot(x, Data3, 'g*')
#plt.xscale('log')
#plt.yscale('log')
plt.title('tamanios vs indice')


plt.show()

