let sigimoide x = atan x;;
let lineaire x = x+.0.5;;
let logistique x = 1./.(1.+.exp (-.x));;
let heaviside x = if x>=0. then 1. else 0.;;

type Neurone = { 
	w0 : float; (* coef de biais *)
	f : float -> float; (* fonction d'activation *)
	mutable poids : float vect;
};;

let creerNeurone nEntree f w0 =
	let n = {w0=w0; f=f; poids=make_vect nEntree 0.0} in
	for i = 0 to nEntree-1 do
		n.poids.(i) <- 0.5;
	done;
	n;;

let resultatNeurone neur entrees = 
	let a = ref 0. in
	for i = 0 to vect_length entrees -1 do
		a := !a +. neur.poids.(i)*.entrees.(i);
	done;
	neur.f (!a-.neur.w0);;

let apprentissageNeurone neur apprentiss alpha (* apprentissage 1 neurone par W-H *)= 
	for k = 0 to (vect_length apprentiss - 1) do
		let e_k, y_k = apprentiss.(k) in
		let s_k = resultatNeurone neur e_k in
		for i = 0 to (vect_length e_k - 1) do
			neur.poids.(i) <- neur.poids.(i) +. alpha*.(y_k-.s_k)*.e_k.(i);
		done;
	done;
	();;

let enseignerNEx neur classe n alpha =
	for i = 1 to n do
		let a = random__int (vect_length classe) in
		apprentissageNeurone neur [|classe.(a)|] alpha;
	done;
	neur;;

type Reseau1Couche = { (* Réseau à une couche cachée *)
	nbEntrees : int;
	nbSorties : int;
	
	mutable nbNeurCache : int;(* Nombre de neurones par couche cachée *)
	mutable neuronesCaches : Neurone vect; (* tous les neurones des couches cachées -> liste car on peut ajouter des neurones quand on veux *)
	
	mutable neurSorties : Neurone vect; (* neurone de sortie *)
};;

let creerReseau nEntrees nSorties nNeurCache f =
	{
		nbEntrees = nEntrees;
		nbSorties = nSorties;
		
		nbNeurCache = nNeurCache;
		
		neuronesCaches = make_vect nNeurCache {w0=0.5; f=sigimoide; poids=(make_vect nEntrees 0.5)};
		
		neurSorties = make_vect nSorties {w0=0.5; f=lineaire; poids=(make_vect nNeurCache 0.5)};
	};;

let resultatReseau reseau entrees =
	let tabCoucheCachee = make_vect reseau.nbNeurCache 0. in 
	for i =0 to (reseau.nbNeurCache-1) do
		tabCoucheCachee.(i) <- (resultatNeurone (reseau.neuronesCaches.(i)) entrees);
	done;
	let out = make_vect reseau.nbSorties 0.0 in
	for i =0 to (reseau.nbSorties-1) do
		out.(i) <- (resultatNeurone (reseau.neurSorties.(i)) tabCoucheCachee);
	done;
	out;;

let print_vect v = 
	for i = 0 to vect_length v -1 do
		print_float v.(i);
		print_string "\n";
	done;;

let apprentissageReseau1Couche reseau apprentiss alpha =
	for a = 0 to (vect_length apprentiss -1) do
		print_int 0;
		let exemple_x, exemple_y = apprentiss.(a) in
		let outTab = make_vect (reseau.nbNeurCache+reseau.nbSorties) 0.0 in
		let d = make_vect (reseau.nbNeurCache+reseau.nbSorties) 0.0 in
		for i = 0 to (reseau.nbSorties-1) do (* on remplit le tableau d -> sortie *)
			let si = (resultatReseau reseau exemple_x).(i) in
			outTab.(i) <- si;
			d.(i) <- si*.(1.-.si)*.(exemple_y.(i)-.si);
		done;
		for i = 0 to (reseau.nbNeurCache-1) do (* on remplit le tableau d -> cache *)
			let oi = (resultatNeurone reseau.neuronesCaches.(i) exemple_x) and
			somme = ref 0. in
			outTab.(reseau.nbSorties+i) <- oi;
			for k = 0 to reseau.nbEntrees-1 do
				somme := !somme +. d.(k)*.reseau.neuronesCaches.(i).poids.(k);
			done;
			d.(reseau.nbSorties+i) <- oi*.(1.-.oi)*.(!somme);
		done;
		for i = 0 to (reseau.nbSorties-1) do (*on actualise les poids -> réseau de sortie*)
			for k = 0 to (reseau.nbNeurCache-1) do
				reseau.neurSorties.(i).poids.(k) <- reseau.neurSorties.(i).poids.(k)+. alpha*.d.(i)*.outTab.(reseau.nbSorties+k);
			done;
		done;
		for i = 0 to (reseau.nbNeurCache-1) do (*on actualise les poids -> réseau cachés*)
			for k = 0 to (reseau.nbEntrees-1) do
				reseau.neuronesCaches.(i).poids.(k) <- reseau.neuronesCaches.(i).poids.(k)+. alpha*.d.(reseau.nbSorties+i)*.outTab.(k);
			done;
		done;
	done;;
