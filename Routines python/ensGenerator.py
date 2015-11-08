import random

def fonction(vect):
	return float(((vect[0]-0.5)**2 + (vect[1]-0.5)**2) <= 0.2**2) #un disque de rayon 0.2 centré en (0.5, 0.5)

random.seed()

nomFichier = "disqueApprent"

nombreIn = 2
nombreOut = 1

nombreEx = 500

with open(nomFichier, "w") as fichier:
	fichier.write(str(nombreIn) + ' ' + str(nombreOut) )
	fichier.write('\n')
	fichier.write(str(nombreEx))
	fichier.write('\n')
	fichier.write('\n')
	for _ in range(nombreEx):
		couple = [random.uniform(-0.2, 1.2), random.uniform(-0.2, 1.2)]
		fichier.write(str(couple[0]) + ' ' + str(couple[1]))
		fichier.write('\n')
		r = fonction(couple)
		print(r)
		fichier.write(str(r))
		fichier.write('\n')
		fichier.write('\n')
