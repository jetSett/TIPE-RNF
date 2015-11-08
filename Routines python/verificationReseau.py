from RNF import Reseau, Ensemble, vec

nomRes = 'ReseauZero250nc'

print('Chargement')

ens = Ensemble('../data/verif_0')
ens2 = Ensemble('../data/apprentiss_0')
res = Reseau(nomRes)

print('Verification')

print(res.verification(ens2))
print(res.verification(ens))
