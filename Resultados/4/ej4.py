
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = 'zetas.txt'
archivo1 = 'tamanios_s1.txt'
archivo2 = 'tamanios_s2.txt'
archivo3 = 'tamanios_s3.txt'

import os
os.system('rm '+ archivo)
os.system('rm '+ archivo1)
os.system('rm '+ archivo2)
os.system('rm '+ archivo3)

os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import rc

Data1 = np.genfromtxt(archivo1, delimiter = '\t')
Data2 = np.genfromtxt(archivo2, delimiter = '\t')
Data3 = np.genfromtxt(archivo3, delimiter = '\t')
zetas = np.genfromtxt(archivo, delimiter = '\t')

plt.plot(zetas, Data1, 'b.', label = 's = 15')
plt.plot(zetas, Data2, 'r*', label = 's = 17')
plt.plot(zetas, Data3, 'gv', label = 's = 19')
plt.grid()
plt.legend(loc = 'best')
plt.title('f(z) vs z')
plt.ylabel(r'$\frac{n_s(p)}{n_s(p_c)}$')
plt.xlabel(r'$z = s^\sigma \cdot \frac{p-p_c}{p_c}$')

plt.figure()
plt.plot(zetas, np.log(Data1), 'b.', label = 's = 15')
plt.plot(zetas, np.log(Data2), 'r*', label = 's = 17')
plt.plot(zetas, np.log(Data3), 'gv', label = 's = 19')
plt.grid()
plt.legend(loc = 'best')
plt.xlabel(r'$z$')
plt.ylabel(r'$log f(z)$')
plt.title('log f(z) vs z')

plt.show()

