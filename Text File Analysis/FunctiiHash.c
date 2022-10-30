#include "header.h"

int codHash (void* elem) {
    char* cuvant = (char*) elem;
    if (cuvant[0] >= 'A' && cuvant[0] <= 'Z')
	    return *cuvant - 'A';
    
    if (cuvant[0] >= 'a' && cuvant[0] <= 'z')
	    return *cuvant - 'a';

    return -1;
}

TH* InitTH(TFHash fh) {
    size_t M = ('Z'-'A');

    TH* h = (TH*) calloc(sizeof(TH), 1);
    if (!h) {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLG*) calloc(M + 1, sizeof(TLG));
    if(!h->v) {
        printf("eroare alocare vector de pointeri TLG in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;
    return h;
}

// compares length between char* and TCelulaNr*
int comparareLungime1 (void* e1, void* e2) {
    char* cuv = (char*) e1;
    TCelulaNr* cel = (TCelulaNr*) e2;

    return (strlen(cuv) == cel->lungimeCuvant);
}

// compares length between 2 TCelulaNr*
int comparareLungime2 (void* e1, void* e2) {
    TCelulaNr* cel1 = (TCelulaNr*) e1;
    TCelulaNr* cel2 = (TCelulaNr*) e2;

    int len1 = cel1->lungimeCuvant;
    int len2 = cel2->lungimeCuvant;

    if (len1 < len2)
        return -1;
    if (len1 == len2)
        return 0;
    if (len1 > len2)
        return 1;

    return -2;  
}


// adds 'elem' in hashtable "tabelaHash"
int InsTH (TH* tabelaHash, void* elem, TFCmp compare) {
    int cod = tabelaHash->fh(elem);
    TLG p = tabelaHash->v[cod];
    for (; p != NULL; p = p->urm) {
        // verifies if a suitable sublist already exists
        if (compare (elem, p->info) == 1) {
            // verifies if count increments or a new cell will be added 
            actualizareSublista(p->info, elem);
            return 1;
        }
    }
    // if it gets here means a new cell must be added containing the number of letters in a word
    int rez = adaugaLista (tabelaHash->v + cod, elem, comparareLungime2);
    return rez;
}

void afi_elem(void* elem) {
    TCelulaNr* celNr = (TCelulaNr*) elem;
    printf("(%d:", celNr->lungimeCuvant);
    TLG p = celNr->urm;
    
    TCuvant* cuvant1 = (TCuvant*) p->info;
    printf("%s/%d", cuvant1->cuv, cuvant1->count);
    p = p->urm;
    
    for (; p != NULL; p = p->urm) {
        TCuvant* cuvant = (TCuvant*) p->info;
        printf(", %s/%d", cuvant->cuv, cuvant->count);
    }
    printf(")");
}

void AfiTH (TH* tabelaHash, TF afi_elem) {
    for (int i = 0; i <= tabelaHash->M; i++) {
        TLG p = tabelaHash->v[i];
        if (p) {
            printf("pos %d: ", i);
            for (TLG el = p; el != NULL; el = el->urm)
                afi_elem(el->info);
            printf("\n");
        }
    }
}


// sorts elements of hashtable
//1. by count
//2. by strcmp
void SortTH (TH* tabelaHash) {
    for (int i = 0; i <= tabelaHash->M; i++) {
        TLG p = tabelaHash->v[i];
        if (p) {
            for (TLG el = p; el != NULL; el = el->urm) {
                sortLG(el->info, comparareSort);
            }
        }
    }
}

void afi_elemMax(void* elem, int n, int i, int* countPos) {
    int countElem = 0;
    
    
    TCelulaNr* celNr = (TCelulaNr*) elem;
    TLG p = celNr->urm;
    
    TCuvant* cuvant1 = (TCuvant*) p->info;
    if (cuvant1->count <= n) {  // first word of sublist is ok
        if (*countPos == 0) {
            printf("pos%d: ", i);
            *countPos = 1;
        }

        printf("(%d: %s/%d", celNr->lungimeCuvant, cuvant1->cuv, cuvant1->count);
        countElem = 1;

        p = p->urm;
    }
    
    for (; p != NULL; p = p->urm) {
        TCuvant* cuvant = (TCuvant*) p->info;
        if (cuvant->count <= n) {
            if (*countPos == 0) {   // first printed element
                printf("pos%d: ", i);
                *countPos = 1;
            } 

            if (countElem == 0) {   // first printed element from sublist
                printf("(%d: %s/%d", celNr->lungimeCuvant, cuvant->cuv, cuvant->count);
                countElem = 1;
            } else {    // needs to be printed but it's not the first
                printf(", %s/%d", cuvant->cuv, cuvant->count);
            }

            
        }
    }
    if (countElem > 0) // there are elements already printed
        printf(")");
}

void afisareMax (TH* tabelaHash, int n) {
    
    
    for (int i = 0; i <= tabelaHash->M; i++) {
        int countPos = 0;    // counts how many elements meet the rule from every position of hashtable array

        TLG p = tabelaHash->v[i];
        if (p) {
            for (TLG el = p; el != NULL; el = el->urm) {
                afi_elemMax(el->info, n, i, &countPos);

            }
            if (countPos > 0)
                printf("\n");
        }
    }
}
