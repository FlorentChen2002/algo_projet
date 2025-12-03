#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//utf8
typedef struct Noeud {
    int frequence;//char
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
        printf("trouvé\n");
        *noeuds_target=noeuds;
    }
    printf("val : %d\n",noeuds->frequence);
    printf("val : %c\n",noeuds->symbole);
    printf("%d\n",noeuds->gdbh);
    if(noeuds->gdbh==4)printf("pk %d\n",noeuds->frequence);
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

void update_GDBH(Noeud* n, int* cpt) {
    if (n == NULL) return;
    
    update_GDBH(n->gauche, cpt);
    update_GDBH(n->droite, cpt);
    n->gdbh = *cpt;
    (*cpt)++;
}

void numerotation_GDBH(ArbreH* arbre) {
    int cpt = 1;
    update_GDBH(arbre->racine,&cpt);
}

void echange_noeud(ArbreH* h, Noeud* n1, Noeud* n2){
    //echange de place entre 2 noeuds
    if (h == NULL || n1 == NULL || n2 == NULL ||n1==n2) return; 
    
    Noeud* parent1 = n1->parent;
    Noeud* parent2 = n2->parent;

    int n1_gauche = (parent1 && parent1->gauche == n1);
    int n2_gauche = (parent2 && parent2->gauche == n2);
    
    if (n1_gauche)parent1->gauche = n2;
    else parent1->droite  = n2;
    
    if (n2_gauche)parent2->gauche = n1;
    else parent2->droite  = n1;
    
    n1->parent = parent2;
    n2->parent = parent1;
    
    int gdbh1 = n1->gdbh;
    int gdbh2 = n2->gdbh;
    n1->gdbh = gdbh2;
    n2->gdbh = gdbh1;
}

void traitement(ArbreH* h, Noeud* q) {
    if (q == NULL ||h == NULL) return;
    if (q == h->racine){
        q->frequence++;
        return;
    }
    printf("===========\n");
    Noeud* b = finBloc(h, q->frequence);
    if (b != q && q->parent != b){
        printf("échange\n");
        echange_noeud(h, q, b);
    }
    q->frequence++; 
    traitement(h, q->parent);
}

void modification(ArbreH* h,unsigned char s,int val) {
    Noeud* q = h->tab_noeud[s];
    if (q == NULL) {
        
        Noeud* ancien_special = h->special;
        Noeud* nouveau_special = buildNoeud(-2, 0, ancien_special,1);
        Noeud* nouvelle_feuille = buildNoeud(val, 1,ancien_special,1);
        
        ancien_special->gauche = nouveau_special;
        ancien_special->droite = nouvelle_feuille;
        ancien_special->symbole = -1;
        ancien_special->frequence = 0;
        
        h->special = nouveau_special;
        h->tab_noeud[s] = nouvelle_feuille;
        
        numerotation_GDBH(h);
        traitement(h, ancien_special);
    }
    else {
        traitement(h, q);
    }
}

void ecrire_binaire(FILE *f, unsigned char *buffer, int *nb_binaire, int binaire) {
    *buffer = (*buffer) << 1;
    if (binaire) *buffer = (*buffer) | 1;
    (*nb_binaire)++;
    if (*nb_binaire == 8) {
        fputc(*buffer, f);
        *buffer = 0;
        *nb_binaire = 0;
    }
}

void ecrire_octet(FILE *f, unsigned char *buffer, int *nb_binaire, unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        int binaire = (c >> i) & 1;
        ecrire_binaire(f, buffer, nb_binaire, binaire);
    }
}

void ecrire_huffman(FILE *f, unsigned char *buffer, int *nb_binaire, Noeud *n) {
    int chemin[256]; 
    int profondeur = 0;

    while (n->parent != NULL) {
        if (n->parent->gauche == n) {
            chemin[profondeur] = 0;
        } else {
            chemin[profondeur] = 1;
        }
        profondeur++;
        n = n->parent;
    }
    for (int i = profondeur - 1; i >= 0; i--) {
        ecrire_binaire(f, buffer, nb_binaire, chemin[i]);
    }
}

void compression(const char* path_txt, const char* path_bin) {
    /*Prend en argument un fichier txt et un fichier bin
      Il encode un fichier txt qui contient des 0 et 1 en un fichier biniaire
    */
    FILE *f_txt = fopen(path_txt, "r");
    FILE *f_bin = fopen(path_bin, "wb");
    if (f_txt == NULL || f_bin == NULL)return;
    unsigned char buffer = 0;
    int nb_binaire = 0;
    ArbreH *arbre = buildArbreH();
    char c;
    while ((c = fgetc(f_txt)) != EOF) {//lecture du fichier txt, un caractere par caractere
        unsigned char symbole = (unsigned char)c;
        Noeud *feuille = arbre->tab_noeud[symbole];
        printf("symbole : %c\n",symbole);
        if (feuille != NULL) {
            ecrire_huffman(f_bin, &buffer, &nb_binaire, feuille);
            modification(arbre, symbole, symbole);
        }else{
            ecrire_huffman(f_bin, &buffer, &nb_binaire, arbre->special);
            ecrire_octet(f_bin, &buffer, &nb_binaire, symbole);
            modification(arbre, symbole, symbole);
        }
    }
    if (nb_binaire > 0) {//si le dernier octet a assez de bits sinon on ajoute les 0 restant
        buffer = buffer << (8 - nb_binaire);
        fwrite(&buffer, 1, 1, f_bin);
    }
    numerotation_GDBH(arbre);
    printf("%d\n",arbre->racine->gauche->gdbh);
    fclose(f_txt);
    fclose(f_bin);
    return;
}
void lecture(const char *path) {
    /*Prend un fichier bin en argument et affiche le contenu du fichier*/
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        printf("Le fichier %s n'a pas pu être ouvert\n\n", path);
        return;
    }
    unsigned char buffer;
    while (fread(&buffer, 1, 1, f) == 1) {//lit un octet par octet
        for (int i = 7; i >= 0; i--) {//convertir en 8 bits
            int binaire = (buffer >> i) & 1; // decalage + mask 
            printf("%d", binaire);
        }
    }
    fclose(f);
    return;
}

int main() {
    compression("text.txt","bin.bin");
    printf("Hello world!\n");
    lecture("bin.bin");
    return 0;
}
//01100011 0 01100001 00 01110010 11 000
//01100011 0 01100001 00 01110010 11 100 01101101 1100 01100010 0 11000110101
//01100011 0 01100001 00 01110010 11 100 01101101 1100 01100010 0 110
