import numpy as np
import matplotlib.pyplot as plt

plt.rcParams["savefig.dpi"] = 300
plt.rcParams["axes.prop_cycle"] = plt.cycler(color=plt.cm.viridis.colors[::45])

exact = -2 + 2 * np.cos(np.arange(6, 0, -1) * np.pi / 7)
values = np.loadtxt("../data/eigenvalues.txt", usecols=range(6), delimiter=",")
Q = np.loadtxt("../data/eigenvectors.txt", usecols=range(6), delimiter=",")

for k in range(6):
    plt.plot(np.loadtxt(f"../data/eigenvalue{k + 1}.txt", usecols=0, delimiter=","), label=f"λ{k + 1}")
plt.xscale("log")
plt.xlabel("Iteration")
plt.ylabel("Eigenvalue estimate")
plt.legend()
plt.savefig("../img/eigenvalue_history.png")
plt.clf()

for k in range(6):
    history = np.loadtxt(f"../data/eigenvalue{k + 1}.txt", usecols=0, delimiter=",")
    plt.semilogy(np.abs(history - exact[k]) + 1e-17, label=f"λ{k + 1}")
plt.xlabel("Iteration")
plt.ylabel("|λ - λ exact|")
plt.legend()
plt.savefig("../img/eigenvalue_error.png")
plt.clf()

x = np.linspace(0, 7, 200)
fig, axes = plt.subplots(2, 3, figsize=(12, 6))
for k, ax in enumerate(axes.flat):
    # q1 is sin(6x*pi/7), q6 is sin(x*pi/7); match the sign of the first point
    ax.plot(x, np.sign(Q[k][0]) * np.sqrt(2 / 7) * np.sin((6 - k) * x * np.pi / 7), color="gray")
    ax.plot(range(1, 7), Q[k], "o", color=f"C{k}")
    ax.set_title(f"q{k + 1}, λ = {values[k]:.4f}")
fig.tight_layout()
fig.savefig("../img/eigenvectors.png")
