#!/bin/python3
import re
import sys, os
#main_file_name = input("Main file:\n>")
os.chdir('Flow')
main_file_name = sys.argv[1]
outf = open("result.cpp", "w")
included = set({main_file_name})

def work(main_file_name, outf):
    try:
        mf = open(main_file_name, encoding="utf-8-sig", mode="r")
    except OSError:
        #print ("No such file")
        return
    for s in mf.readlines():
        #print (s)
        if s.startswith("#pragma once"):
            continue
        elif s.startswith('#include "'):
            name = s.split('"')[1].split('.')[0]
            print (name)
            if not (name+'.h' in included):
                included.add(name+'.h')
                work(name+'.h', outf)
            if not name+'.cpp' in included:
                included.add(name+'.cpp')
                work(name+'.cpp', outf)
        else:
            outf.write(s)
    outf.write('\n')

work(main_file_name, outf)
outf.close()

