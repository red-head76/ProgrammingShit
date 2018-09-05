# Copyright 2018 Luis Restat
# Using various code snippets from
# http://ad-wiki.informatik.uni-freiburg.de/teaching

import os
import re

while (True):
    os.system("echo")    
    userinput = input("Instance Number:")
    def files(path):  
        for file in os.listdir(path):
            if os.path.isfile(os.path.join(path, file)):
                yield file

    def printInstance(instancenum):
        
        instances = files("./instances/")
        rx = re.compile(r'i[0]{0,2}' + instancenum + '-n[\S]*.plain')
        for instance in instances:
            m = rx.search(instance)
            if (m != None):
                instancename = "./HashiSolverMain ./instances/" + m.group(0)
                os.system(instancename)

    s1 = re.search(r'\A[\d]{0,3}\Z', userinput)
    s2 = re.search(r'([\d]{0,3})-([\d]{0,3})', userinput)
    
    if (s1):
        printInstance(userinput)
        
    if (s2):
        i1 = int(s2.group(1))
        i2 = int(s2.group(2)) + 1
        if (i1 <= i2):
            for i in range(i1, i2):
                print("# " + str(i))
                printInstance(str(i))
                print("\n")
