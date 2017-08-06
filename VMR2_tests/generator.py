#!/usr/bin/env python
# -*- coding: utf-8 -*-

from decimal import Decimal
import random
import math
import matplotlib.pyplot as plt
import os

'''
This file is generator of test data for VMR2 software. Python 2.7
It generates data file with VMR2 format header and some data, then it plot it by matplotlib.
'''

# count of dots in test data
COUNT_OF_DOTS = 500000
# absolute max x value
MEASURE_TIME_S = 60.0
# noise values
MAX_VAL = 1
MIN_VAL = -1
# step by time
X_STEP = MEASURE_TIME_S/COUNT_OF_DOTS
# name of file with test data
FILENAME = "test_f_.txt"

if __name__ == "__main__":
    '''
    Test data generator for VMR2
    '''
    with open(FILENAME, 'w') as outfile:
        
        # form header
        outfile.write("#Organozation Name, SoftVersion\n")
        outfile.write("#Voltage by ADC\n")
        outfile.write("#06.08.2017 16:10\n")
        
        l = list()
        
        # first half of data
        for i in range(0, COUNT_OF_DOTS/2):
            (x, y) = i * X_STEP, math.sin(2*math.pi*i*X_STEP) + random.uniform(MIN_VAL, MAX_VAL) + 3*MAX_VAL
            l.append((x, y))
        
        # second half of data
        for i in range(COUNT_OF_DOTS/2, COUNT_OF_DOTS):
            (x, y) = i * X_STEP, math.sin(2*math.pi*i*X_STEP*2)/2 + random.uniform(2*MIN_VAL, 2*MAX_VAL) + 3*MAX_VAL
            l.append((x, y))
            
        l = sorted(l, key=lambda x: x[0])
        
        for (x,y) in l:
            outfile.write("{:.3E} {:.3E}\n".format(x,y))        
        
    with open(FILENAME, 'r') as f:
        content = f.readlines()

        X = list()
        Y = list()
            
        for s in content:            
            if len(s) > 0:
                if '#' is not s[0]:
                    try:
                        s_array = s.split(' ')
                        if len(s_array) == 2:
                           x = float(s_array[0])
                           y = float(s_array[1])

                           X.append(x)
                           Y.append(y)
                    except:
                        continue
        plt.figure()
        plt.plot(X, Y, 'r--')                
        plt.savefig("test_f_.png")
