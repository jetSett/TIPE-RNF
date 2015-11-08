(* -------------------------------------------------------fonction OU -----------------------------------------------------------------------------------------*)
let cours = [| [|1.; 0.|], 1.; [|1.; 1.|], 1.; [|0.; 0.|], 0.; [|0.; 1.|], 1.; [|0.5; 0.|], 1.; [|0.0; 0.5|], 1.; [|0.; 0.2|], 0.; [|0.2; 0.|], 0.;|];;
let epsilon = 0.05;;
let neur = creerNeurone 2 heaviside 0.5;;


print_string "Fonction OU\nAvant apprentissage\n";;
resultatNeurone neur [|0.; 1.|];;

resultatNeurone neur [|1.; 0.|];;
resultatNeurone neur [|0.; 0.5|];;

apprentissageNeurone neur cours epsilon 500 appAdaline;;
print_string "Fonction OU\nAprès apprentissage\n";;

resultatNeurone neur [|0.; 1.|];;

resultatNeurone neur [|1.; 0.|];;
resultatNeurone neur [|0.; 0.5|];;

(* ----------------------------------------------------------fonction ET--------------------------------------------------------------------------------------- *)

let neur = creerNeurone 2 heaviside 0.5;;
let epsilon = 0.05;;
let cours = [| [|1.; 0.|], 0.; [|1.; 1.|], 1.; [|0.; 0.|], 0.; [|0.; 1.|], 0.; [|0.5; 0.|], 0.; [|0.0; 0.5|], 0.; [|0.8; 0.3|], 1.; [|0.3; 0.8|], 1.; |];;

print_string "Fonction ET\nAvant apprentissage\n";;
resultatNeurone neur [|1.; 1.|];;

resultatNeurone neur [|1.; 0.1|];;
resultatNeurone neur [|0.8; 0.5|];;

apprentissageNeurone neur cours epsilon 500 appAdaline;;
print_string "Fonction ET\nAprès apprentissage\n";;

resultatNeurone neur [|1.; 1.|];;

resultatNeurone neur [|1.; 0.1|];;
resultatNeurone neur [|0.8; 0.5|];;
