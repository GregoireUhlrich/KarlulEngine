import numpy as np
import os


os.system("make clean")
headers = [f for f in os.listdir("include/") if os.path.isfile(os.path.join("include/", f))]
sources = [f for f in os.listdir("src/") if os.path.isfile(os.path.join("src/", f))]

lines_headers = 0
lines_sources = 0
listClasses = np.array([],dtype="string");
print "\nHeaders: \n"
for h in headers:
    f = open("include/"+h, "r")
    i = 0
    for line in f:
        i += 1
        tmp = np.array(line.split(), dtype='string')
        a, = np.where(tmp == "class")
        if (len(a) > 0):
            b, = np.where(listClasses == tmp[a[0]+1])
            if (len(b) == 0):
                listClasses = np.hstack((listClasses, tmp[a[0]+1]))
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
print "\nTotal: -",lines_headers+lines_sources,"lines."
print "\n       -",len(listClasses),"classes."
