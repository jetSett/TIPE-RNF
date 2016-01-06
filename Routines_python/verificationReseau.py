from RNF import Reseau, Ensemble, vec

nomRes = 'ReseauMultiSortie'
print('Chargement')

ens = Ensemble('../data/apprent_all')
ens2 = Ensemble('../data/verif_all')
res = Reseau(nomRes)

print('Verification')

print(str(res.verification(ens2)*100/ens.taille())+'%')
print(str(res.verification(ens)*100/ens.taille()) + '%')
