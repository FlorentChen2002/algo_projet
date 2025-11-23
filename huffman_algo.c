#include <stdio.h>
#include <stdlib.h>

typedef struct Noeud {
    int frequence;
    int symbole;
    //la recherche par enfant gauche ou droite O(n)
    struct Noeud *parent;//pred
    struct Noeud *gauche;//next
    struct Noeud *droite;//next
    int gdbh;
} Noeud;

typedef struct {
    Noeud *racine;
    Noeud *special; //acces a la # , O(1)
    Noeud *tab_noeud[256];// un tas , mais je n'ai pas encore regarder comment faire le lien avec l'arbre et le tab
} ArbreH;

Noeud* buildNoeud(int symbole, int freq, Noeud* parent,int cpt){
    //cree un noeud
    Noeud* noeuds = malloc(sizeof(Noeud));
    if(!noeuds)return NULL;
    noeuds->symbole = symbole;
    noeuds->frequence = freq;
    noeuds->parent = parent;
    noeuds->gauche = NULL;
    noeuds->droite = NULL;
    noeuds->gdbh = cpt;
    return noeuds;
}

ArbreH* buildArbreH(){
    //initialise un arbre de huffman
    ArbreH* arbre = malloc(sizeof(ArbreH));
    if(!arbre)return NULL;
    arbre->racine = buildNoeud(-1,0,NULL,0);// # = -1
    arbre->special = arbre->racine;
    for (int i = 0; i<256; i++)arbre->tab_noeud[i]=NULL;
    return arbre;
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
