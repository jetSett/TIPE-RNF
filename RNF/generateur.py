import random
n = 500

with open('ensembleXOR.txt', 'w') as fichier:
	 fichier.write('2 1\n{}\n\n'.format(n))
	 for i in range(n):
		 a = random.choice([0, 1])
		 b = random.choice([0, 1])
		 fichier.write('{} {}\n{}\n\n'.format(a, b, int( (a and not b) or ((not a) and b) )) )
	 
