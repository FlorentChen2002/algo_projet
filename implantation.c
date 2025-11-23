#include <stdio.h>
#include <stdlib.h>

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

void ecriture(const char* path_txt, const char* path_bin) {
    /*Prend en argument un fichier txt et un fichier bin
      Il encode un fichier txt qui contient des 0 et 1 en un fichier biniaire
    */
    FILE *f_txt = fopen(path_txt, "r");
    FILE *f_bin = fopen(path_bin, "wb");
    if (f_txt == NULL || f_bin == NULL) {
        printf("Le fichier n'a pas pu être ouvert\n");
        return;
    }
    unsigned char buffer = 0;
    int nb_binaire = 0;
    char c;
    while ((c = fgetc(f_txt)) != EOF) {//lecture du fichier txt, un caractere par caractere
        if (c == '0' || c == '1') {
            buffer = buffer << 1;//decalage vers la gauche de 1 (le caractere par defaut : 00000000)
            if (c == '1') {
                buffer = buffer | 1;//remplace le dernier bit par un 1
            }
            nb_binaire++;
            if (nb_bits == 8) {//ecriture dans le fichier bin quand il y a 8 bits
                fwrite(&buffer, 1, 1, f_bin);
                nb_binaire = 0;
                buffer = 0;
            }
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

int main(int argc, char *argv[]) {
    return 0;
}
