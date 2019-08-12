
""" client.py - Echo client for sending/receiving C-like structs via socket
References:
- Ctypes fundamental data types: https://docs.python.org/2/library/ctypes.html#ctypes-fundamental-data-types-2
- Ctypes structures: https://docs.python.org/2/library/ctypes.html#structures-and-unions
- Sockets: https://docs.python.org/2/howto/sockets.html
"""

import socket
import sys
import ctypes
import os
import subprocess
from contextlib import closing
import inspect

#%%
class Directories:
    PARENT_FOLDER = os.path.dirname(os.getcwd())#parent folder
    
    FOLDER_ALGO_C = "Algorithmes_C"
    FOLDER_GRAPH_C = "Graph"
    FOLDER_PYTHON_C = "Py_interface"
    NAME_MAIN_SERVER_C = "main_py"
    
    FODLER_C = os.path.join(PARENT_FOLDER, FOLDER_ALGO_C)
    FOLDER_C_GRAPH = os.path.join(FODLER_C, FOLDER_GRAPH_C)
    FOLDER_C_PY = os.path.join(FODLER_C, FOLDER_PYTHON_C)

#%%
class Task:
    '''task to send to the server'''
    t_closeServer=-1
    t_buildTree=0
    t_stronglyConnectedGraph=1
    t_randomGraph=2
    t_free=3
    t_petri=4
    t_freeChoice=5


#%%
class Request(ctypes.Structure):
    _fields_ = [("task", ctypes.c_int), 
                ("n", ctypes.c_uint),
                ("D", ctypes.c_uint),
                ("wrapperId", ctypes.c_uint),
                ("newWrapperId", ctypes.c_uint),
                ("Ki", ctypes.c_int),
                ("Ko", ctypes.c_int),
                ("isTree", ctypes.c_uint),
                ("rep_vect_norm", ctypes.c_uint)
               ]
    ''' must be at the same order than the field of the c structure'''
    
def createRequest(task, n=None, D=None, wrapperId=None, isTree=None, newWrapperId=None, Ki=None, Ko=None, rep_vect_norm=None):
    '''create request with the correct format for the arguments'''
    frame = inspect.currentframe()
    args, _, _, values = inspect.getargvalues(frame)#get arguments of the function
    
    for i in args:
        name_args = i
        value = values[i]
        
        if(name_args=="task"):
            task = ctypes.c_int(value)
        elif(name_args=="n"):
            if(value is None):
                n = ctypes.c_uint(0)
            else :
                n = ctypes.c_uint(value)
        elif(name_args=="D"):
            if(value is None):
                D = ctypes.c_uint(0)
            else :
                D = ctypes.c_uint(value)
        elif(name_args=="wrapperId"):
            if(value is None):
                wrapperId = ctypes.c_uint(0)
            else :
                wrapperId = ctypes.c_uint(value)
        elif(name_args=="isTree"):
            if(value is None):
                isTree = ctypes.c_uint(0)
            else :
                isTree = ctypes.c_uint(value)
        elif(name_args=="newWrapperId"):
            if(value is None):
                newWrapperId = ctypes.c_uint(0)
            else :
                newWrapperId = ctypes.c_uint(value)
        elif(name_args=="Ki"):
            if(value is None):
                Ki = ctypes.c_int(-1)
            else :
                Ki = ctypes.c_int(value)
        elif(name_args=="Ko"):
            if(value is None):
                Ko = ctypes.c_int(-1)
            else :
                Ko = ctypes.c_int(value)
        elif(name_args=="rep_vect_norm"):
            if(value is None):
                rep_vect_norm = ctypes.c_uint(0)
            else :
                rep_vect_norm = ctypes.c_uint(value)
        
    return Request(task, n, D, wrapperId, newWrapperId, Ki, Ko, isTree, rep_vect_norm)

#%%
class Server:

    def __init__(self, port=None, buffersize=512):
        if(port is None):
            self.PORT = Server.findFreePort()
        else :
            self.PORT = port
        self.BUFFERSIZE = buffersize
        self.HOSTNAME = 'localhost'
        self.FOLDER_C = Directories.FODLER_C
        self.NAME_MAIN_SERVER_C = Directories.NAME_MAIN_SERVER_C
        
        #start C server
        self.openCServerSide()
    
    @staticmethod
    def findFreePort():
        with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as s:
            s.bind(('', 0))
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            return s.getsockname()[1]
    
    def openCServerSide(self):
        '''Launch the C functions that will create the socket server and 
           answer the request of the client side'''
        function = os.path.join(self.FOLDER_C, self.NAME_MAIN_SERVER_C)
        args=[self.PORT,self.BUFFERSIZE]
        
        args=[str(arg) for arg in args]
        
        txt = ""
        for arg in args:
           txt = txt +arg 
        txt = function +" "+ txt
        print("Commande lance sur le terminal : "+ txt)
        print("\tParms : Port="+str(self.PORT)+", Buffersize="+str(self.BUFFERSIZE))
        
        args.insert(0, function)
        subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        #Popen(['/bin/sh', '-c', args[0], args[1], ...])
        
    
    def close(self):
        '''close C socket server'''
        request = Request(Task.t_closeServer)
        response = self.getResponse(request)
        
        if(len(response)==0):
            print("Fermeture du serveur reussie")
        else :
            print("un probleme est survenu lors de la fermeture du server")
    
    
    def getResponse(self, request):
        '''send request and retrieve the response as string'''
        txt = ""
        #start socket
        server_addr = (self.HOSTNAME, self.PORT)
        sok = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        try:
            sok.connect(server_addr)
        except:
            print("ERROR: Connection to %s refused" % repr(server_addr))
            sys.exit(1)
    
        try:
            #send request
            sok.send(request)
            
            #waiting for the response
            count = 0
            while count==0 or txt[count-1]!='\x00':#'\0'
                buff = sok.recv(ctypes.sizeof(ctypes.c_char)*self.BUFFERSIZE)
                buff = buff.decode('utf8')
                count = count + len(buff)
                txt= txt+buff
                
            txt = txt[:count-1]#response of teh server side
                
        finally:
            sok.close()
        
        return txt

#%%
#request = Request(Task.f_createStronglyConnectedGraph, ctypes.c_uint(100), ctypes.c_uint(8))
#buff1 = Socket.getResponse(request)
