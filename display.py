import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def fill(start, finish, count):
    if (start == finish):
        return np.full(count, start)
    else:
        return np.arange(start, finish, (finish-start)/count)
# 10,000 samples per meaning unit
# each meaning unit will consist of 3 stages
# that result in a unique vibration pattern
t = fill(0, 100, 10000)
s = np.concatenate((fill(0, 100, 3333), fill(100,100,3333), fill(100,0,3334)))

fig, ax = plt.subplots()
ax.plot(t, s)

ax.set(xlabel='time (% out of meaning unit)', ylabel='frequency (Hz)',
       title='vibration speaker frequency')
ax.grid()

plt.show()