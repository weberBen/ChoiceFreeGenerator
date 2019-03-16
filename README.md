# Creation of random free choice graph generator


# Sommaire
- [Description](#description)
- [Interface](#Interface)


# Description <a name="Description"/>

A free choice graph is a subclass of pretri net where each place has exactly one output and transitions have one input and multiple outputs as shown on the following figure

<img src="Annexes/Images/FreeChoiceDef.png" width="70%"  align="middle">

# Interface <a name="Interface"/>

All the functions about generating and modifying graph are written in C. But all the display part is a script python (because python can use networkx for oriented graph and SNAKES for petri net). Thus, we had to connect C functions with python. So simplest solution for now is to create a server socket in C that collects python request, process the request and send back the result to python; ctypes was kind of unsatisfying. All the described process can be visualize on the following figure :

<img src="Annexes/Images/Py_interface.png" width="60%"  align="middle">

  - # Command to use python interface
