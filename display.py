import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def fill(start, finish, count):
    if (start == finish):
        return np.full(count, start)
    else:
        return np.arange(start, finish, (finish-start)/(count-0.5))

# 10,000 samples per meaning unit
# each meaning unit will consist of 3 epochs
# that result in a unique vibration pattern
def gen_data(epochs):
    data = []
    for epoch in epochs:
        if (epoch == "L"):
            data = np.append(data, fill(40, 40, 3333))
        elif (epoch == "H"):
            data = np.append(data, fill(150,150,3333))
        elif (epoch == "LH"):
            data = np.append(data, fill(40,150,3333))
        elif (epoch == "HL"):
            data = np.append(data, fill(150,40,3333))
    data = np.append(data, data[9998])
    data = (fill(0,100,10000), data)
    return data

def disp_data(t, s):
    fig, ax = plt.subplots() 
    ax.plot(t, s)
    ax.set(xlabel='Time (% of meaning unit interval)', ylabel='Frequency (Hz)',
       title='Vibration Speaker Frequency')
    plt.ylim(35,155)
    ax.grid()
    plt.show()

epochs = ["L", "H", "L"]
(t,s) = gen_data(epochs)
disp_data(t,s)