#!/usr/bin/env python3
import twlang
import os
import sys

os.chdir(os.path.dirname(__file__) + "/../..")

if len(sys.argv) < 3:
    print("usage: python copy_fix.py <infile> <outfile> [--delete-unused] [--append-missing] [--delete-empty]")
    sys.exit()
infile = sys.argv[1]
outfile = sys.argv[2]
args = sys.argv[3:]
delete_unused = False
append_missing = False
delete_empty = False
for arg in args:
    if arg == "--delete-unused":
        delete_unused = True
    elif arg == "--append-missing":
        append_missing = True
    elif arg == "--delete-empty":
        delete_empty = True
    else:
        print("No such argument '"+arg+"'.")
        sys.exit()

content = open(infile).readlines()
trans = twlang.translations(infile)
if delete_unused or append_missing:
    local = twlang.localizes()
if append_missing:
    supported = []
for tran, (start, expr, end) in trans.items():
    if delete_unused and tran not in local:
        content[start:end] = [None]*(end-start)
    if append_missing and tran in local:
        if expr or (not expr and delete_empty):
            supported.append(local.index(tran))
        else:
            content[start:end] = [None]*(end-start)
    if delete_empty and not expr:
        content[start:end] = [None]*(end-start)
content = [line for line in content if line != None]
if append_missing:
    missing = [index for index in range(len(local)) if index not in supported]
    if missing:
        if content[-1] != "\n":
            content.append("\n")
        for i, miss in enumerate(missing):
            content.append(local[miss]+"\n== \n\n")
        content[-1] = content[-1][:-1]

open(outfile, "w").write("".join(content))
print("Successfully created '"+outfile+"'.")