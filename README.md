# algo_projet

##Introduction


1)Montrer que l’algorithme de modification de l’arbre n’échange jamais un noeud avec un des ses ancêtres.


-Définition du poids dans un arbre de Huffman : 
	Dans tout arbre de huffman, le poids d’un noeud parent est la somme des poids des 
	enfants : W(Parent) = W(EnfantDroite) + W(EnfantGauche)
	Donc le poids du parent est toujour supérieur ou égal au poids de l’enfant : 
	W(Parent) >= W(Enfant)
	
-Condition nécessaire pour faire l’échange : 
	On peut faire un échange entre deux noeuds s’ils respectent la propriété GDBH. 
	Donc pour qu’un échange est lieu entre un noeud Q et son ancêtre A, il faut que : 
	W(Q) = W(A)

-Les 2 cas possible en respectant la définition et la condition : 
	-Le premier cas, ou le poids de son frère est supérieur à 0 alors on a  W(A) > W(Q)
	Donc l’échange ne serait pas possible, car l’ancêtre A est plus lourd que celui de Q.
	-Le deuxième cas, ou le poids de son frère est égale à 0 alors on a W(A) = W(Q)
	Ils ont le même poids mais l’échange sera bloqué par le code : 
	Dans la page 47 du cours, la condition empêche l’échange avec le parent :
	
		if enfants(parent(Q))=={Q,#} and parent(Q) == finBloc(H,Q):
			Ajouter 1 au poids de Q
			Q = parent(Q) #il envoie son père au lieu du fils au traitement


-Conclusion : mathématiquement est impossible et dans l’algorithme, il est explicitement interdit de faire l’échange avec son ancêtre. Donc on peut faire l’échange de place avec son “oncle” ou un “cousin” en respectant les règles énumérées.

2)Quel est le nombre maximum d’échanges réalisés lors d’une modification de l’arbre (en fonction de la taille de l’alphabet des symboles) ? 

L’algorithme de modification remonte le chemin de la feuille vers la racine et à chaque étape il effectue un seul échange pour rétablir la propriété AHA. Le nombre d'échanges est donc borné par la hauteur de l’arbre. Et dans le pire des cas, un arbre de Huffman fait une “peigne” qui a une hauteur de n . Donc le nombre maximum d’échanges réalisés lors d’une modification de l’arbre est O(n), où n est la taille de l’alphabet des symboles.
