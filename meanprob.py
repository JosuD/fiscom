# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 17:00:32 2017

@author: JosuD
"""

import os
L = input('Lado de la red: ')
print(' ')
Z = input('Iteraciones: ')
print(' ')

archivo = "L"+L+".txt"
salida = "salida"+archivo

os.system('rm '+ archivo)
os.system('./a.out '+ L + ' ' + Z + ' ' + archivo)
os.system('gedit '+archivo)


import numpy as np
Data = np.genfromtxt(archivo, delimiter = '\t')
#print(type(Data), type(Data[5]))
print('Encontre N = ', np.shape(Data)[0], ' valores, cuyo valor medio es ', np.mean(Data), ' y su desviacion ', np.std(Data))
matris = np.array([float(L), np.mean(Data), np.std(Data)])
#print(type(matris), type(matris[0]), type(matris[1]), type(matris[2]), np.shape(matris))
np.savetxt(salida, matris, delimiter = '\t')


