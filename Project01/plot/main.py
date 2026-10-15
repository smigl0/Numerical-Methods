import matplotlib.pyplot as plt

data_gamma_2=[]
with open("../data/gamma2.txt") as f:
    for i,record in enumerate(f):
        data_gamma_2.append(float((record.rstrip())))

data_gamma_3=[]
with open("../data/gamma3.txt") as f:
    for i,record in enumerate(f):
        data_gamma_3.append(float((record.rstrip())))

fig, (ax1, ax2) = plt.subplots(1,2,figsize=(12,6))

ax1.plot(range(len(data_gamma_2)), data_gamma_2)
ax1.set_title("Gamma = 2")
ax1.set_xlabel("Iteration")
ax1.set_ylabel("Norm")

ax2.plot(range(len(data_gamma_3)), data_gamma_3)
ax2.set_title("Gamma = 3")
ax2.set_xlabel("Iteration")
ax2.set_ylabel("Norm")

plt.tight_layout()
plt.savefig('plot.png') 