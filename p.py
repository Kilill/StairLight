#!/usr/bin/python3
from math import sin
i=1
while int(pow(2,i/90)) <1:
        i+=1
print('progmem int tabel[');
c=0
for x in range(i,1024+i):
    y=int(pow(2,x/90))
    print("{: >4d}".format(y),end="")
    c+=1
    if x<i+1024:
        print (",",end="")
    if c%12 == 0:
        print("")

print("]")
print("#define MAXINTENS {0}".format(y))

