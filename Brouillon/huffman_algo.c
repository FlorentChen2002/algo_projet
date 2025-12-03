#include <stdio.h>
#include <stdlib.h>

#define SPECIALES -2
#define NOEUD_INT -1
//utf8
typedef struct Noeud {
    int frequence;//char
    int symbole;
    //la recherche par enfant gauche ou droite O(n)
    struct Noeud *parent;
    struct Noeud *gauche;//next
    struct Noeud *droite;//next
} Noeud;

typedef struct {
    Noeud *racine;
    Noeud *special; //acces a la # , O(1)
    Noeud *tab_feuille[256];// un tas , mais je n'ai pas encore regarder comment faire le lien avec l'arbre et le tab
    Noeud *liste_noeud[512];//l ensemble des noeuds
    int nb_noeuds;// combien de noeud il y adans liste_noeud
} ArbreH;

Noeud* buildNoeud(int symbole, int freq,Noeud *parent ,int cpt){
    //cree un noeud
    Noeud* noeuds = malloc(sizeof(Noeud));
    if(!noeuds)return NULL;
    noeuds->symbole = symbole;
    noeuds->frequence = freq;
    noeuds->parent = parent;
    noeuds->gauche = NULL;
    noeuds->droite = NULL;

    return noeuds;
}

ArbreH* buildArbreH() {
    ArbreH* arbre = calloc(1,sizeof(ArbreH));
    if (!arbre) return NULL;
    Noeud* n = buildNoeud(SPECIALES,0,NULL,0);
    if (!n){
        free(arbre);
        return NULL;
    }
    arbre->racine = n;
    arbre->special = n;
    arbre->liste_noeud[0]=n;
    arbre->nb_noeuds = 1;
    return arbre;
}

int trouver_index(ArbreH* h, Noeud* n) {
    //retourner l'index du noeud
    for (int i = 0; i < h->nb_noeuds; i++) {
        if (h->liste_noeud[i] == n) return i;
    }
    return -1;
}

Noeud* finBloc(ArbreH* h, Noeud* m) {
    //retourner le noeud qui a la meme frequence et un classe superieur a lui
    int i = trouver_index(h, m);
    int poids = m->frequence;
    Noeud* noeuds_target= NULL;

    while (i < h->nb_noeuds) {
        Noeud* candidat = h->liste_noeud[i];
        if(candidat->frequence == poids){
            noeuds_target=candidat;
        }
        i++;
    }
    return noeuds_target;
}

void echange_noeud(ArbreH* h, Noeud* n1, Noeud* n2){
    //on echange de place 2 noeuds
    if (h == NULL || n1 == NULL || n2 == NULL ||n1==n2) return; 
    
    Noeud* parent1 = n1->parent;
    Noeud* parent2 = n2->parent;

    int n1_gauche = (parent1 && parent1->gauche == n1);
    int n2_gauche = (parent2 && parent2->gauche == n2);
    int idx_n1 = trouver_index(h, n1);
    int idx_n2 = trouver_index(h, n2);

    h->liste_noeud[idx_n1] = n2;
    h->liste_noeud[idx_n2] = n1;

    if (parent1) {
        if (n1_gauche) parent1->gauche = n2;
        else parent1->droite = n2;
    } else h->racine = n2;
    
    if (parent2) {
        if (n2_gauche) parent2->gauche = n1;
        else parent2->droite = n1;
    } else h->racine = n1;
    
    n1->parent = parent2;
    n2->parent = parent1;
}

void traitement(ArbreH* h, Noeud* q) {
    //verifie si il respect les propriete de huffman sinon il le corrige
    if (q == NULL ||h == NULL) return;
    if (q == h->racine){
        q->frequence++;
        return;
    }
    Noeud* b = finBloc(h, q);
    if (b != q && q->parent != b){
        echange_noeud(h, q, b);
    }
    q->frequence++; 
    traitement(h, q->parent);
}

int main() {
    //test
    ArbreH* arbre = buildArbreH();
    Noeud* noeud1 = buildNoeud(101,1,NULL,1);
    arbre->racine = noeud1;
    Noeud* noeud2 = buildNoeud(103,1,noeud1,1);
    Noeud* noeud3 = buildNoeud(104,1,noeud1,1);
    noeud1->gauche = noeud2;
    noeud1->droite = noeud3;
    noeud3->gauche = arbre->special;
    echange_noeud(arbre,noeud2,noeud3);
    printf("%d\n",arbre->racine->symbole);
    printf("%d\n",arbre->racine->gauche->symbole);
    printf("%d\n",arbre->racine->droite->symbole);
}
