from RNF import Reseau, Ensemble, vec


from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.gca(projection='3d')
X = np.arange(-0.05, 1.1, 0.1)
Y = np.arange(-0.05, 1.1, 0.1)

Z = [ [i]*len(Y) for i in range(len(X))]

res = Reseau('ReseauXOR')

i= 0
for x in X:
	j = 0
	for y in Y:
		v = vec(str(x) + ' ' + str(y))
		Z[i][j] = float(str(res.resultat(v)))
		j += 1
	i += 1

X, Y = np.meshgrid(X, Y)


surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm,
        linewidth=0, antialiased=False)
        
ax.set_zlim(-0.1, 1.1)


fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()

