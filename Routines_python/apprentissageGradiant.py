import math

from RNF import Reseau, Ensemble, vec


nomRes = 'ReseauXOR'

print('Chargement')

ens = Ensemble('xor.exemples')
res = Reseau(nomRes)
epsilon = 0.01
errOld = 1000

print('début')

i = 0
n = 200

#~ erreurPreced = 50
for _ in range(n):
	err = res.apprentGrad(ens, epsilon)
	if math.isnan(err):
		break
	delta = errOld-err
	print(str((err/2)*100/ens.taille()) + '% d\'erreur | Progression : ' + str(i*100/n) + '%')
	errOld = err
	ens.shuffle()
	i+=1
print(res.verification(ens))
res.save(nomRes)
