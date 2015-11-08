from RNF import Reseau, Ensemble, vec

nomRes = 'ReseauZero250nc'

print('Chargement')

ens = Ensemble('../data/apprentiss_0')
res = Reseau(nomRes)
epsilon = 0.1
errOld = 1

print('début')

i = 0
n = 1
#~ erreurPreced = 50
for _ in range(n):
	err = res.apprentGrad2(ens, epsilon)
	delta = errOld-err
	print(str(err) + '  ' + str(i*100/n) + '%')
	errOld = err
	ens.shuffle()
	i+=1
#~ print(res.print_resultat(ens))
res.save(nomRes)
