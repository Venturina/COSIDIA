#/usr/bin/python3
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


data = {'Time': [],
        'loss': [] }

df = pd.DataFrame(data)

print(df)


filepath = '../build.ninja/error.log'
with open(filepath) as fp:
    line = fp.readline()
    cnt = 1
    while line:
        l = line.split()
        if len(l) > 13:
            #print(l)
            curTime = int(l[11])
            if curTime > 3005000:
                df = df.append({'Time':curTime, 'loss':int(l[15]) * -1}, ignore_index=True)
        line = fp.readline()
        cnt += 1

print(cnt)
print(df)
lines = df.plot.line(x='Time', y='loss')
lines.plot()

plt.show()
#print(df)