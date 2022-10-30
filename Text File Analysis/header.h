#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>


typedef int (*TFHash)(void*);
typedef int (*TFCmp)(void*, void*);
typedef void (*TF)(void*);  


typedef struct celulag {    
  void* info;                   // stores info
  struct celulag *urm;          // pointer to next cell
} TCelulaG, *TLG;

typedef struct {                // stores length of words found at address urm
    int lungimeCuvant;
    struct celulag* urm;
} TCelulaNr;

typedef struct {                // sublist data type
    char* cuv;                  // stores the words and their number of appearances
    int count;
} TCuvant;

typedef struct {                // hashtable
    size_t M;
    TFHash fh;
    TLG *v;
} TH;

int codHash (void* elem);   // hash function
TH* InitTH(TFHash fh);      // memory allocation for hashtable
int InsTH (TH* tabelaHash, void* elem, TFCmp compare);
int adaugaLista (TLG* Lista, void* elem, TFCmp compare);
int comparareLungime1 (void* e1, void* e2);
int comparareLungime2 (void* e1, void* e2);
int actualizareSublista (void* lista, void* elem);
void AfiTH (TH* tabelaHash, TF afi_elem);
void afi_elem(void* elem);
int comparareSort (void* e1, void* e2);
void sortLG(void* el, TFCmp compare);
void SortTH (TH* tabelaHash);
int litera(char c);
int numar(char c);
int stringToInt(char* s);
void afisareCod(TLG L, char c, int n);
void afisareMax (TH* tabelaHash, int n);
void afi_elemMax(void* elem, int n, int i, int* count);
void DistrLg (void* L);
int cuvant(char* cuv);
char MajMin (char c);

