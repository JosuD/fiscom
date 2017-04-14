# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 17:00:32 2017

@author: JosuD
"""

import numpy as np
from matplotlib import pyplot as plt
plt.close()

Data = np.genfromtxt("myfile.txt", delimiter = '\t')
#x = np.arange(np.shape(Data[0,:])[0])
#y = np.arange(np.shape(Data[:,0])[0])
#yy, xx = np.meshgrid(x,y)
#plt.pcolor(xx,yy,Data)
plt.imshow(Data, filternorm = 99)
plt.colorbar()
plt.show()


