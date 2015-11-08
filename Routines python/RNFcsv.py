from RNF import Reseau, Ensemble, vec
import numpy as np
from os import system

X = np.arange(-1, 1, 0.1)
Y = np.arange(-1, 1, 0.1)

Z = [ [i]*len(Y) for i in range(len(X))]

res = Reseau('Res2')

i= 0
for x in X:
	j = 0
	for y in Y:
		v = vec(str(x) + ' ' + str(y))
		Z[i][j] = float(str(res.resultat(v)))
		j += 1
	i += 1

string = "Y/X;"

for x in X:
	string += str(x) + ';'
string += '\n'
i = 0
for y in Y:
	string += str(y) + ';'
	for z in Z[i]:
		string += str(z)+';'
	string += '\n'
	i+=1

system('echo "{}" > test.csv'.format(string))
