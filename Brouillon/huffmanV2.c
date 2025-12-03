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

ArbreH* buildArbreH(){
    //initialise un arbre de huffman
    ArbreH* arbre = malloc(sizeof(ArbreH));
    if(!arbre)return NULL;
    arbre->racine = buildNoeud(SPECIALES,0,NULL,0);// # = -1
    arbre->special = arbre->racine;
    arbre->nb_noeuds=1;
    for (int i = 0; i<256; i++){
        arbre->liste_noeud[i]=NULL;
        arbre->tab_feuille[i]=NULL;
    }
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

void modification(ArbreH* h, unsigned char s, int val) {
    //ajout d'une nouvelle feuille ou met a jour la feuille
    Noeud* q = h->tab_feuille[s];

    if (q == NULL) {//si le mot n'est pas encore enregistrer
        
        Noeud* ancien_special = h->special; 

        Noeud* nouveau_special = buildNoeud(SPECIALES, 0, ancien_special, 0);
        Noeud* nouvelle_feuille = buildNoeud(val, 1, ancien_special, 0);
        ancien_special->gauche = nouveau_special;
        ancien_special->droite = nouvelle_feuille;
        ancien_special->symbole = NOEUD_INT;
        ancien_special->frequence = 0;
        
        h->special = nouveau_special;
        h->tab_feuille[s] = nouvelle_feuille;
        for (int i = h->nb_noeuds - 1; i >= 0; i--) {
            h->liste_noeud[i + 2] = h->liste_noeud[i];
        }

        h->liste_noeud[0] = nouveau_special;
        h->liste_noeud[1] = nouvelle_feuille;
        h->liste_noeud[2] = ancien_special;
        
        h->nb_noeuds += 2;

        traitement(h, ancien_special);
    }

    else {
        traitement(h, q);
    }
}

void ecrire_binaire(FILE *f, unsigned char *buffer, int *nb_binaire, int binaire) {
    //fait le decalage a gauche et mask
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
    //transforme l'octet en binaire
    for (int i = 7; i >= 0; i--) {
        int binaire = (c >> i) & 1;
        ecrire_binaire(f, buffer, nb_binaire, binaire);
    }
}

void ecrire_huffman(FILE *f, unsigned char *buffer, int *nb_binaire, Noeud *n) {
    //lit l'arbre et retourne le resultat
    int msg_binaire[256]; 
    int profondeur = 0;

    while (n->parent != NULL) {
        if (n->parent->gauche == n) {
            msg_binaire[profondeur] = 0;
        } else {
            msg_binaire[profondeur] = 1;
        }
        profondeur++;
        n = n->parent;
    }

    for (int i = profondeur - 1; i >= 0; i--) {
        ecrire_binaire(f, buffer, nb_binaire, msg_binaire[i]);
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
        Noeud *feuille = arbre->tab_feuille[symbole];
        if (feuille != NULL) {//si le mot est deja dans enregistrer alors on ecrit la version compresser
            ecrire_huffman(f_bin, &buffer, &nb_binaire, feuille);
            modification(arbre, symbole, symbole);
        }else{//sinon on ecrire le code # et l'ascii du caractere
            ecrire_huffman(f_bin, &buffer, &nb_binaire, arbre->special);
            ecrire_octet(f_bin, &buffer, &nb_binaire, symbole);
            modification(arbre, symbole, symbole);
        }
    }

    if (nb_binaire > 0) {//si le dernier octet a assez de bits sinon on ajoute les 0 restant
        buffer = buffer << (8 - nb_binaire);
        fwrite(&buffer, 1, 1, f_bin);
    }

    fclose(f_txt);
    fclose(f_bin);
    return;
}
void lecture(const char *path) {
    /*Prend un fichier bin en argument et affiche le contenu du fichier*/
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        printf("Le fichier %s n'a pas pu etre ouvert\n\n", path);
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
    compression("carambarbcm.txt","carambarbcm.huff");
    lecture("carambarbcm.huff");
    return 0;
}
//carambarbcm : 0110 0011 0 0110 0001 00 0111 0010 11 100 0110 1101 1100 0110 0010 0 111 1001 1001 1001 0000
//              0110 0011 0 0110 0001 00 0111 0010 11 100 0110 1101 1100 0110 0010 0 111 1001 1001 1001 0000
