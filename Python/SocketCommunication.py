
""" client.py - Echo client for sending/receiving C-like structs via socket
References:
- Ctypes fundamental data types: https://docs.python.org/2/library/ctypes.html#ctypes-fundamental-data-types-2
- Ctypes structures: https://docs.python.org/2/library/ctypes.html#structures-and-unions
- Sockets: https://docs.python.org/2/howto/sockets.html
"""

import socket
import sys
import random
import ctypes
import struct

BUFFERSIZE = 12
f_createTree = 0

""" This class defines a C-like struct """
#%%
class Task:
    closeServer=-1
    f_createTree=0
    f_createStronglyConnectedGraph=1
    f_createNonVolatileTree=2
    f_freeNonVolatileGraph=3
    f_stronglyConnectedOnNonVolatileGraph=4
    
#%%
class Request(ctypes.Structure):
    _fields_ = [("task", ctypes.c_int), 
                ("n", ctypes.c_uint),
                ("D", ctypes.c_uint)]

#%%
def getResponse(request):
    server_addr = ('localhost', 2400)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    txt = ""
    
    try:
        s.connect(server_addr)
    except:
        print("ERROR: Connection to %s refused" % repr(server_addr))
        sys.exit(1)

    try:
        #send request
        s.send(request)
        
        #waiting for the response
        count = 0
        while count==0 or txt[count-1]!='\x00':#'\0'
            buff = s.recv(ctypes.sizeof(ctypes.c_char)*BUFFERSIZE)
            buff = buff.decode('utf8')
            count = count + len(buff)
            txt= txt+buff
            
        txt = txt[:count-1]#response of teh server side
            
    finally:
        s.close()
    
    return txt


request = Request(Task.f_createNonVolatileTree, ctypes.c_uint(100), ctypes.c_uint(8))
buff1 = getResponse(request)

request = Request(Task.f_createStronglyConnectedGraph, ctypes.c_uint(45), ctypes.c_uint(7))
buff4 = getResponse(request)

request = Request(Task.f_stronglyConnectedOnNonVolatileGraph)
buff2 = getResponse(request)

request = Request(Task.f_createNonVolatileTree, ctypes.c_uint(10), ctypes.c_uint(3))
buff6 = getResponse(request)

request = Request(Task.closeServer, ctypes.c_uint(100), ctypes.c_uint(8))
buff5 = getResponse(request)
