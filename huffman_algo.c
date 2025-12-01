#include <stdio.h>
#include <stdlib.h>

typedef struct Noeud {
    int frequence;
    int symbole;
    //la recherche par enfant gauche ou droite O(n)
    struct Noeud *parent;
    struct Noeud *gauche;//next
    struct Noeud *droite;//next
    int gdbh;
} Noeud;

typedef struct {
    Noeud *racine;
    Noeud *special; //acces a la # , O(1)
    Noeud *tab_noeud[256];// un tas , mais je n'ai pas encore regarder comment faire le lien avec l'arbre et le tab
} ArbreH;

Noeud* buildNoeud(int symbole, int freq,Noeud *parent ,int cpt){
    //cree un noeud
    Noeud* noeuds = malloc(sizeof(Noeud));
    if(!noeuds)return NULL;
    noeuds->symbole = symbole;
    noeuds->frequence = freq;
    noeuds->gauche = NULL;
    noeuds->droite = NULL;
    noeuds->gdbh = cpt;
    return noeuds;
}

void recherche_finbloc(Noeud* noeuds, Noeud** noeuds_target, int freq_target){
    //recherche du meilleur target dans l'arbre si il y a une égalité il retourne le gdbh le plus haut
    if (noeuds == NULL)return;
    if(noeuds->frequence == freq_target && (*noeuds_target == NULL || noeuds->gdbh>(*noeuds_target)->gdbh)){
        *noeuds_target=noeuds;
    }
    recherche_finbloc(noeuds->droite,noeuds_target,freq_target);
    recherche_finbloc(noeuds->gauche,noeuds_target,freq_target);
    return;
}

Noeud* finBloc(ArbreH* arbre, int freq){
    //Permet de lancer la fonction recherche_finbloc
    Noeud* target = NULL;
    recherche_finbloc(arbre->racine,&target,freq);
    return target;
}

void echange_noeud(ArbreH* H, Noeud* n1, Noeud* n2){
    //echange de place entre 2 noeuds
    Noeud* parent1 = n1->parent;
    Noeud* parent2 = n2->parent;
    if (parent1) {
        if (parent1->gauche == n1) parent1->gauche = n2;
        else parent1->droit  = n2;
    }
    if (parent2) {
        if (parent2->gauche == n2) parent2->gauche = n1;
        else parent2->droit  = n1;
    }
    n1->parent = parent2;
    n2->parent = parent1;
    if (H->racine == n1) H->racine = n2;//cas si le parent est la racine
    else if (H->racine == n2) H->racine = n1;
}
