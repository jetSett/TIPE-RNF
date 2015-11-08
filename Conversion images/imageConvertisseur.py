from PIL import Image

name = "0.png"

im = Image.open(name)

box = []

n = 10

largeur_portion = int(im.size[0]/n)
hauteur_portion = int(im.size[1]/n)

for i in range(n):
	for j in range(n):
		box.append(im.crop(
		(largeur_portion*i, hauteur_portion*j,
		largeur_portion*(i+1), hauteur_portion*(j+1))
		))


for i in range(n*n):
		total = 0
		for data in box[i].getdata():
			total += (3*255-data[0]-data[1]-data[2])/3
		print(total/(largeur_portion*hauteur_portion*255))
