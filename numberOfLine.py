import numpy as np
import os

os.system("make clean")
headers = [f for f in os.listdir("include/") if os.path.isfile(os.path.join("include/", f))]
sources = [f for f in os.listdir("src/") if os.path.isfile(os.path.join("src/", f))]

lines_headers = 0
lines_sources = 0

print "\nHeaders: \n"
for h in headers:
    f = open("include/"+h, "r")
    i = 0
    for line in f:
        i += 1
    f.close()
    lines_headers += i
    print h,":",i,"lines."

print "\nSub total headers:",lines_headers,"lines.\n\n\nSources:\n"
for s in sources:
    f = open("src/"+s, "r")
    i = 0
    for line in f:
        i += 1
    f.close()
    lines_sources += i
    print s,":",i,"lines."
    
print "\nSub total sources:",lines_sources,"lines."
print "\nTotal:",lines_headers+lines_sources,"lines."
