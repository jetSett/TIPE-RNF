import random

def fonction(vect):
	x, y = vect
	return int( (0.25 <= x <= 0.75) and (0.25 <= x <= 0.75))

random.seed()

nomFichier = "xor.exemples"

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
		couple = [random.uniform(0.0, 1.0), random.uniform(0.0, 1.0)]
		fichier.write(str(couple[0]) + ' ' + str(couple[1]))
		fichier.write('\n')
		r = fonction(couple)
		print(r)
		fichier.write(str(r))
		fichier.write('\n')
		fichier.write('\n')
