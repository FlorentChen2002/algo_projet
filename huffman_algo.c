#include <stdio.h>
#include <stdlib.h>

#define SYMBOLE_SPECIAL -1

typedef struct Noeud {
    int frequence;
    int symbole;
    //la recherche par enfant gauche ou droite O(n)
    struct Noeud *parent;//pred
    struct Noeud *gauche;//next
    struct Noeud *droit;//next
} Noeud;

typedef struct {
    Noeud *racine;
    Noeud *special; //acces a la # , O(1)
    Noeud *tab_noeud[256];// un tas , mais je n'ai pas encore regarder comment faire le lien avec l'arbre et le tab
} ArbreH;

Noeud* buildNoeud(int symbole, int freq, Noeud* parent){
    Noeud* noeuds = malloc(sizeof(Noeud));
    if(!noeuds)return NULL;
    noeuds->symbole = symbole;
    noeuds->frequence = freq;
    noeuds->parent = parent;
    noeuds->gauche = NULL;
    noeuds->droite = NULL;
    return noeuds;
}

ArbreH* buildArbreH(){
    ArbreH* arbre = malloc(sizeof(ArbreH));
    if(!arbre)return NULL;
    arbre->racine = buildNoeud(SYMBOLE_SPECIAL,1,NULL);
    arbre->special = arbre->racine;
    for (int i = 0; i<256; i++)arbre->tab_noeud[i]=NULL;
    return arbre;
}
