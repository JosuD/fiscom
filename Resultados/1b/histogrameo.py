
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = 'histograma.txt'
archivo2 = 'intensidades.txt'

import os
os.system('rm '+ archivo)
os.system('rm '+ archivo2)

os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)
#os.system('gedit '+archivo)


import numpy as np
from matplotlib import pyplot as plt
from matplotlib import rc

Data = np.genfromtxt(archivo, delimiter = '\t')
Data2 = np.genfromtxt(archivo2, delimiter = '\t')

print('Encontre N = ', np.shape(Data)[0], ' valores, cuyo valor medio es ', np.mean(Data), ' y su desviacion ', np.std(Data))

bines = np.linspace(0,1,len(Data))
y = np.zeros(len(Data))+0.5

a = Data[0]
dist = 0.5
for i in range(1,len(Data)):
	if(np.abs(Data[i]-0.5)<dist):
		a = Data[i]
		dist = np.abs(Data[i]-0.5)
		print("paso el " + str(i))
		print(str(np.abs(Data[i]-0.5)), " vs ", str(np.abs(Data[i-1]-0.5)))
indice = Data.tolist().index(a)
print(a, indice)

plt.plot(bines, Data, 'r.', bines, y, 'k-')
plt.plot(bines[indice], Data[indice], 'g*')
plt.grid(True)
plt.xlabel("Probabilidad")
plt.ylabel("F(p)")
plt.title(r' L = ' + L + '$\; p_c = %.2f\pm %.2f$' %(bines[indice],bines[2]-bines[1]))

plt.figure()
plt.plot(bines, Data2, 'r.')
plt.title('Intensidad del cluster')

plt.show()

