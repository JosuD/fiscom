
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = 'tamanios.txt'

import os
os.system('rm '+ archivo)
os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)


import numpy as np
from matplotlib import pyplot as plt
from scipy.optimize import curve_fit
plt.close()

Data = np.genfromtxt(archivo, delimiter = '\t')
s = np.linspace(0, len(Data),len(Data))
f = lambda x, a, b: a*x+b
popt, pcov = curve_fit(f, np.log(s[1:]), np.log(Data[1:]))

plt.plot(s, Data, 'b.')
plt.grid()
plt.title('n_s(pc) vs s')

plt.figure()
plt.plot(np.log(s[1:]), np.log(Data[1:]), 'r.', label = "Datos")
plt.plot(np.log(s[1:]), f(np.log(s[1:]), popt[0], popt[1]), 'b-', label = "Ajuste")
plt.grid()
plt.legend(loc = 'best')
plt.title('log ns vs log s')

print(popt, "\n", pcov)

plt.show()

