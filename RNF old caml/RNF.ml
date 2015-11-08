random__init (unix__time ());;

let print_vect v = 
	for i = 0 to vect_length v -1 do
		print_float v.(i);
		print_string "\n";
	done;;


let sigimoide x = atan x;;
let lineaire x = x+.0.5;;
let id x = x;;
let logistique x = 1./.(1.+.exp (-.x));;
let heaviside x = if x>=0. then 1. else 0.;;

type Neurone = { 
	w0 : float; (* coef de biais *)
	mutable f : float -> float; (* fonction d'activation, mutable pour les test*)
	mutable poids : float vect;
};;

let creerNeurone nEntree f w0 =
	let n = {w0=w0; f=f; poids=make_vect nEntree 0.0} in
	for i = 0 to nEntree-1 do
		let p = random__float 1. in (* le poids de départ est choisi aléatoire *)
		n.poids.(i) <- p;
	done;
	n;;

let resultatNeurone neur entrees = 
	let a = ref 0. in
	for i = 0 to vect_length entrees -1 do
		a := !a +. neur.poids.(i)*.entrees.(i);
	done;
	neur.f (!a-.neur.w0);;

let appAdaline neur exemple epsilon =
	let xk, dk = exemple in
	let ek = dk-.(resultatNeurone neur xk) in
	for i = 0 to vect_length neur.poids -1 do
		let gradiant = -.2.*.ek*.xk.(i) in
		neur.poids.(i) <- neur.poids.(i)-.epsilon*.gradiant;
	done;
	();;
	
let apprentissageNeurone neur apprentiss epsilon n methode (* apprentissage 1 neurone par la methode de notre choix *) = 
	for k = 1 to n do
		let i = random__int (vect_length apprentiss) in
		methode neur apprentiss.(i) epsilon;
	done;
	();;

(* ce réseau possède "nBCouches" couches cachées + une couche de sortie *)
(* On spécifie 2 fonctions d'activations : celle des neurones cachés, celle des neurones de sortie *)
(* On a mini une couche cachée *)
type Reseau = { 
	nbEntree: int;
	nbCouches: int;
	nbSorties: int;
	nbNeurParCouche: int vect;
	mutable poids: float vect vect vect; (* poids par couches (1 couche = 1 ligne) /!\ la dernière couche est celle des neurones de sortie *)
	(* On a un tableau en dim 3 : (i, j, k) -> i : couche; j : neurone; k : poids relié au neurone precédent les notations commencent à 0*)
	(* On note que les entrée sont des neurones linéaires : ils renvoient juste leur valeur *)
	(* On note que les coordonnées des coefs de biais sont en i, j, 0 Il y a donc size(couche i-1 )+1 poids dans le tableau *)
	fCache: float -> float; (* usuellement sigimoide*)
	fSortie: float -> float; (* usuellement linéaire/seuil *)	
};;


(* on crée le réseau *)
let creerReseau nbEntree nbCouches nbNeurParCouche fCache nbSorties fSortie =
	let r = {
		nbEntree = nbEntree;
		nbNeurParCouche = nbNeurParCouche;
		nbCouches = nbCouches;
		nbSorties = nbSorties;
		fCache = fCache;
		fSortie = fSortie;
		poids = make_vect (nbCouches+1) (make_vect 2 [|0.; 0.|]); (* On prend aussi les poids des neurones de sortie *)
	} in
	
	(* On initialise les poids de la couche 0 *)
	r.poids.(0) <- make_vect nbNeurParCouche.(0) [|0.|]; (* on initialise tous les neurones *)
	for j = 0 to nbNeurParCouche.(0)-1 do (* pour chaques neurones *)
		r.poids.(0).(j) <- make_vect (nbEntree+1) 0.; (* on crée les poids en mémoire plus 1 pour le coef de biais *)
		for k = 0 to nbEntree do (* on initialise tous les poids en tenant compte du coef de biais *)
			r.poids.(0).(j).(k) <- random__float 1.;
		done;
	done;
	print_string "Couche 0 initialisée\n";
	
	(* on initialise les autres couches sur le même modèle *)
	
	for i = 1 to nbCouches-1 do (* on initialise les couches cachées sans la couche de sortie *)
		r.poids.(i) <- make_vect nbNeurParCouche.(i) [|0.|];
		for j = 0 to nbNeurParCouche.(i)-1 do (* pour chaques neurones *)
			r.poids.(i).(j) <- make_vect (nbNeurParCouche.(i-1)+1) 0.; (* on a autant de poids que de neurones dans la couche inférieure +1 (biais)*)
			for k = 0 to nbNeurParCouche.(i-1) do
				r.poids.(i).(j).(k) <- random__float 1.; (* on initialise le poids en question *)
			done;
		done;
		print_string "Couche "; print_int i; print_string " initialisée\n";
	done;
	let fin = nbCouches in
	r.poids.(fin) <- make_vect nbSorties [|0.|]; (* on initialise les neurones de sortie *)
	for j = 0 to nbSorties-1 do (* pour chaque neurone de sortie *)
		r.poids.(fin).(j) <- make_vect (nbNeurParCouche.(fin-1)+1) 0.; (* on initialise ses poids *)
		for k = 0 to nbNeurParCouche.(fin-1) do
			r.poids.(fin).(j).(k) <- random__float 1.; (* on initialise le k ième poids *)
		done;
	done;
	print_string "Couche "; print_int fin; print_string " initialisée\n";
	r;;

(* le rendu d'un réseau *)
let resultatReseau reseau entrees =
	let sommePondereeAvecBiais entrees poids = (* fonction qui va nous donner les résultats d'un neurones (en fonction de ses poids) *)
		let s = ref 0. in
		for i = 0 to vect_length entrees -1 do
			s := !s+.entrees.(i)*.poids.(i+1);
		done;
		(!s+.(poids.(0)*.0.5)) in
		
	let entreeAct = ref entrees in
	for i = 0 to reseau.nbCouches-1 do (* on fait les poids des neurones cachés *)
		let newEntree = make_vect (vect_length reseau.poids.(i)) 0. in
		
		for j = 0 to vect_length reseau.poids.(i)-1 do
			let somme = sommePondereeAvecBiais !entreeAct reseau.poids.(i).(j) in
			newEntree.(j) <- reseau.fCache somme;
		done;
		entreeAct:= newEntree;
	done;
	let fin = reseau.nbCouches and sorties = make_vect reseau.nbSorties 0. in
	for j = 0 to reseau.nbSorties-1 do
		sorties.(j) <- reseau.fSortie (sommePondereeAvecBiais !entreeAct reseau.poids.(fin).(j) );
	done;
	sorties;;	

(* TODO : implémenter la fonction d'apprentissage *)

let reseau = creerReseau 2 1 [|5|] sigimoide 2 lineaire;;

resultatReseau reseau [|0.3; 0.9|];;
