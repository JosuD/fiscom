
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = 'masa_l.txt'

import os
os.system('rm '+ archivo)
os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)


import numpy as np
from matplotlib import pyplot as plt
plt.close()

Data = np.genfromtxt(archivo, delimiter = '\t')

l = np.arange(len(Data))+1
plt.plot(l*5, Data, 'b.')
plt.grid()
plt.title('Masa en funcion de l')


plt.show()

