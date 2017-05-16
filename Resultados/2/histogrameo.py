
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
from matplotlib import rc
from scipy.optimize import curve_fit

Data = np.genfromtxt(archivo, delimiter = '\t')
Data2 = np.genfromtxt(archivo2, delimiter = '\t')
Data3 = np.genfromtxt(archivo3, delimiter = '\t')

print('Encontre N = ', np.shape(Data)[0], ' valores, cuyo valor medio es ', np.mean(Data), ' y su desviacion ', np.std(Data))

bines = np.linspace(0,1,len(Data))
y = np.zeros(len(Data))+0.5

f = lambda p, A, pc, b: A*(p-pc)**b
popt, pcov = curve_fit(f, bines[60:], Data2[60:], p0 = [1, 0.59, 2])

plt.plot(bines, Data, 'r.', bines, y, 'k-')
plt.grid()
plt.title('Funcion cumulativa')

plt.figure()
plt.plot(bines, Data2, 'r.')
plt.plot(bines[60:], f(bines[60:], *popt), 'b-', label = 'ajuste')
plt.text(0,0.2, r'$A = %.2f \pm %.2f$'%(popt[0],pcov[0,0]), fontsize = 15)
plt.text(0,0.4, r'$p_c = %.2f \pm %.2f$'%(popt[1],pcov[1,1]), fontsize = 15)
plt.text(0,0.6, r'$\beta = %.2f \pm %.2f$'%(popt[2],pcov[2,2]), fontsize = 15)
plt.grid()
plt.legend(loc = "best")
plt.xlabel("Probabilidad")
plt.ylabel(r'$P_\infty$')
plt.title('Intensidad del cluster para L = ' + L)


plt.figure()
x = np.arange(0,len(Data3))
plt.plot(x, Data3, 'g*')
#plt.xscale('log')
#plt.yscale('log')
plt.title('tamanios vs indice')


plt.show()

