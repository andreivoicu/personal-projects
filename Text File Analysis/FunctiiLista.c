#include "header.h"

// adds a cell which stores the number of letters in the word
// and a cell which remembers the number of occurences
int adaugaLista (TLG* Lista, void* elem, TFCmp compare) {
    TLG aux = calloc(1, sizeof(TCelulaG));
	if(!aux)
	    return 0;

    char* cuv = (char*) elem;
    int len = strlen(cuv);

    // the generic cell is allocated
    TLG celCuv = (TLG)calloc(1, sizeof(TCelulaG));

    TCelulaNr* celNr = (TCelulaNr*)calloc(1, sizeof(TCelulaG));

    // the first TCuvant* cell from current sublist is allocated
    TCuvant* cuvant = (TCuvant*)calloc(1, sizeof(TCuvant));
    cuvant->cuv = (char*)calloc(len + 1, sizeof(char));

    if (!celCuv || !celNr || !cuvant || !cuvant->cuv)
        return 0;

    // the word and number of occurences is copied, then stored in "->info"
    strcpy(cuvant->cuv, cuv);
    cuvant->count = 1;
    celCuv->info = (void*) cuvant;

    // the generic cell from the sublist is linked to "->urm" from TCelulaNR* 
    celNr->lungimeCuvant = len;
    celNr->urm = celCuv;


    // insert in hashtable list
	aux->info = (void*) celNr;
	TLG ant = *Lista;
	TLG p = *Lista;
	if (*Lista == NULL) {	// the list is void
        *Lista = aux;
        aux->urm = NULL;
	} else if (compare(aux->info, p->info)<0) {		// insert at the beginning
        aux->urm = *Lista;
        *Lista = aux;
	} else {			// insert inside
		for (; p != NULL; ant = p, p = p->urm) {
			if (compare(aux->info, p->info) < 0) {
				ant->urm = aux;
				aux->urm = p;
				break;
			}
		}

		if (p == NULL) {	// insert at the end
			ant->urm = aux;
			aux->urm = NULL;
		}
	}
	return 1;
}

// count is incremented if the word already exists
// or a new cell is added to the sublist
int actualizareSublista (void* lista, void* elem) {
    TCelulaNr* aL = (TCelulaNr*) lista;
    TLG p = aL->urm;
    TLG ant = p;
    for (; p != NULL; ant = p, p = p->urm) {
        TCuvant* cuvant = (TCuvant*) p->info;
        if (strcmp((char*) elem, cuvant->cuv) == 0) {
            cuvant->count++;
            return 1;
        }
    }

    // a new cell must be added at the end of sublist
    TLG aux = (TLG)calloc(1, sizeof(TCelulaG));
    TCuvant* cuvant = (TCuvant*)calloc(1, sizeof(TCuvant));
    cuvant->cuv = (char*)calloc(strlen ((char*)elem) + 1, sizeof(char));
    if (!aux || !cuvant || !cuvant->cuv)
        return 0;

    strcpy(cuvant->cuv, (char*)elem);
    cuvant->count = 1;
    aux->info = (void*)cuvant;
    aux->urm = NULL;

    ant->urm = aux;

    return 1;
}

// compares 2 "TCuvant" elements. If outcome > 0
// the elements must be interchanged 
int comparareSort (void* e1, void* e2) {
    TCuvant* cuv1 = (TCuvant*) e1;
    TCuvant* cuv2 = (TCuvant*) e2;
    if (cuv1->count < cuv2->count) {    // must be changed
        return 1;
    } else if (cuv1->count == cuv2->count) {
        return strcmp(cuv1->cuv, cuv2->cuv);
    }

    return -1; // for return values < 0 the list is not modified 
}



// sorts a generical list by compare criteria
void sortLG(void* el, TFCmp compare) {
    TCelulaNr* cel = (TCelulaNr*) el;
    
    for (TLG i = cel->urm; i->urm != NULL; i = i->urm) {
        for (TLG j = i->urm; j != NULL; j = j->urm) {
            if (compare(i->info, j->info) > 0) { // must be changed
                void* aux = i->info;
                i->info = j->info;
                j->info = aux;
            }
        }
    }
}

// if c is lowercase returns uppercase 
// if c is uppercase returns lowercase 
char MajMin (char c) {
    if (c >= 'A' && c <= 'Z')   // uppercase
        return (c + 32); 

    if (c >= 'a' && c <= 'z')   // lowercase
        return (c - 32);
}

// prints every word of length n
void afisareCod(TLG L, char c, int n) {
    if (L == NULL) {
        printf("NULL\n");
    }

    for (TLG p = L; p != NULL; p = p->urm) {
        TCelulaNr* cel = (TCelulaNr*) p->info;
        if (cel->lungimeCuvant == n) {  // there are words of length n
            TLG el = cel->urm;
            TCuvant* cuvant = (TCuvant*) el->info;
            printf("(%d:%s/%d", n, cuvant->cuv, cuvant->count);

            el = el->urm;
            while (el != NULL) {
                TCuvant* cuvant1 = (TCuvant*) el->info;
                printf(", %s/%d", cuvant1->cuv, cuvant1->count);

                el = el->urm;
            }
            printf(")\n");
        }
    }


}

// frees memory for Cuvant type
void DistrLg (void* L) {
    TCelulaNr* cel = (TCelulaNr*)L;
    TLG p = cel->urm;
    while (p != NULL) {
        TLG aux = p;
        p = p->urm;

        TCuvant* cuvant = (TCuvant*) aux->info;
        free(cuvant->cuv);
        free(cuvant);
        free(aux); 
    }
    free(p);
    free(cel);
}
