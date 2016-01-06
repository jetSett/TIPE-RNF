
tailleEntree = 784
imagesApprent = []
imagesVerif = []

nombreApprent = 5000
nombreVerif = 5000

with open("train.csv", "r") as fichier:
	print("lecture du fichier")
	fichier.readline()
	i = 0
	for line in fichier:
		tab = line.split(',')
		imagesApprent.append((tab[0], tab[1:]))
		i+=1
		if i > nombreApprent:
			break
	i = 0
	for line in fichier:
		tab = line.split(',')
		imagesVerif.append((tab[0], tab[1:]))
		i+=1
		if i > nombreVerif:
			break

print("écriture des fichiers d'apprentissage")
for a in range(11):
	with open("apprentiss_"+str(a), "w") as fichier:
		fichier.write(str(tailleEntree) + '\n')#nombre d'entrées
		fichier.write('1\n')#nombre de sorties
		fichier.write(str(i) + '\n')#nombre d'exemples
		for exemple in imagesApprent:
			fichier.write(' '.join(exemple[1]))
			fichier.write(str(int(str(a) == exemple[0]))+'\n')		

print("écriture des fichiers de vérification")
for a in range(11):
	with open("verif_"+str(a), "w") as fichier:
		fichier.write(str(tailleEntree) + '\n')#nombre d'entrées
		fichier.write('1\n')#nombre de sorties
		fichier.write(str(i) + '\n')#nombre d'exemples
		for exemple in imagesVerif:
			fichier.write(' '.join(exemple[1]))
			fichier.write(str(int(str(a) == exemple[0]))+'\n')	

print('Ecriture des fichiers multi')
print("écriture des fichiers d'apprentissage")
with open("apprent_all", "w") as fichier:
	fichier.write(str(tailleEntree) + '\n')#nombre d'entrées
	fichier.write('10\n')#nombre de sorties
	fichier.write(str(i) + '\n')#nombre d'exemples
	for exemple in imagesApprent:
		fichier.write(' '.join(exemple[1]))
		fichier.write(' ' + ' '.join([str(int(exemple[0]==str(i))) for i in range(11)]))

print("écriture des fichiers de verification")
with open("verif_all", "w") as fichier:
	fichier.write(str(tailleEntree) + '\n')#nombre d'entrées
	fichier.write('10\n')#nombre de sorties
	fichier.write(str(i) + '\n')#nombre d'exemples
	for exemple in imagesVerif:
		fichier.write(' '.join(exemple[1]))
		fichier.write(' ' + ' '.join([str(int(exemple[0]==str(i))) for i in range(11)]))
