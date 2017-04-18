# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 17:00:32 2017

@author: JosuD
"""

import os
os.system('rm myfile.txt')
os.system('rm percola.txt')

os.system('./a.out')
os.system('gedit myfile.txt')
os.system('gedit percola.txt')

import numpy as np
from matplotlib import pyplot as plt
plt.close()

Data = np.genfromtxt("myfile.txt", delimiter = '\t')
plt.imshow(Data, filternorm = 99)
plt.colorbar()
plt.show()


