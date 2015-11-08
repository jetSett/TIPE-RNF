from RNF import Reseau, Ensemble, vec

def mettreEnForme(d):
	t = ','.join(str(d).split('.'))
	return "\"%s\"" % t

nomRes = 'ReseauZero500lin'

print('Chargement')

ensApprent = Ensemble('../data/apprentiss_0')
ensVerif = Ensemble('../data/verif_0')

res = Reseau(nomRes)

epsilonDepart = 0.01
multEpsilon = 0.70
errOld = 1000000
verif = 0

print('début')

i = 0 #indice de départ
n = 50
#~ erreurPreced = 50

epsilon = epsilonDepart

with open('log_%s.csv' % nomRes, 'w') as fichierLog:
	for _ in range(n):
		err = res.apprentGrad(ensApprent, epsilon)
		delta = errOld-err
		if delta < epsilon:
			epsilon*=multEpsilon
		if epsilon < 1e-4:
			epsilon = epsilonDepart
		errOld = err
		verif = res.verification(ensVerif)
		print("Ensemble d'apprent : {}\nEnsemble de vérif : {}\nEpsilon: {}\n".format(err, verif, epsilon))
		fichierLog.write('{},{},{}\n'.format(i, mettreEnForme(err), mettreEnForme(verif)))
		if err == 0.0:
			print("apprentissage total")
			break
		ensApprent.shuffle()
		i+=1
	#~ print(res.print_resultat(ens))
	i = input("sauver réseau ? (O/n)")
	if i == 'O' or i == 'o':
		res.save(nomRes)
		print('Réseau sauvegardé')

