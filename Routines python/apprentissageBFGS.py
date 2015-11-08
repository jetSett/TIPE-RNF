from RNF import Reseau, Ensemble, vec

nomRes = 'ReseauDisque'

ens = Ensemble('disqueApprent')
res = Reseau(nomRes)
epsilon = 0.0001
errOld = 1

i = 0
n = 1000000
#~ erreurPreced = 50
for _ in range(n):
	err = res.apprentBFGS(ens, epsilon)
	delta = errOld-err
	print(str(err) + '  ' + str(i*100/n) + '%')
	errOld = err
	ens.shuffle()
	i+=1
#~ print(res.print_resultat(ens))
res.save(nomRes)
