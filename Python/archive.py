#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 15 13:20:17 2019

@author: benjamin
"""

import ctypes
import os

PARENT_FOLDER = os.path.dirname(os.getcwd())#parent folder


direc = os.path.join(PARENT_FOLDER, "Algorithmes_C/Librairies/lib.so")
lib = ctypes.CDLL(direc)
#lib.py_createStronglyConnectedGraph(ctypes.c_uint(n), ctypes.c_uint(D), filename.encode('utf-8'))

class Array(ctypes.Structure):
    pass
Array._fields_ = [("data", ctypes.c_int), ("next", ctypes.POINTER(Array))]

filename="test.txt"

lib.py_createTree.restype = ctypes.POINTER(Array)

list_a = ctypes.POINTER(Array)*10
list_a = lib.py_createTree(ctypes.c_uint(10), ctypes.c_uint(5), filename.encode('utf-8'))

p = Array()
p = list_a[0]
#print(int(p.data))


#%%
class Array(ctypes.Structure):
    pass
Array._fields_ = [("data", ctypes.c_int), ("next", ctypes.POINTER(Array))]

def toCharArray(string):
    return string.encode('utf-8')

class StringArray(ctypes.Structure):
    _fileds_=[("data", ctypes.c_char_p),("size", ctypes.c_uint)]