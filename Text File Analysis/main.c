#include "header.h"

// frees the memory allocated for hash table
void quit (TH** h) {
    for (int i = 0; i <= (*h)->M; i++) {
        TLG p = (*h)->v[i];
        while (p != NULL) {
            DistrLg(p->info);
            TLG aux = p;
            p = p->urm;

            free(aux);
        }
        free(p);
    }

    free((*h)->v);
    free(*h);

    exit(0);
}

// tests if c is a letter
int litera(char c) {
    if (c >= 'a' && c <='z')
        return 1;
    if (c >= 'A' && c <='Z')
        return 1;
    
    return 0;
}

// tests if c is a digit
int numar(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

// transforms string to int
int stringToInt(char* s) {
    int rez = 0, i = 0;
    while (s[i] != '\0') {
        rez = 10 * rez + (int)(s[i] - '0');
        i++;
    }
    
    return rez;
}

// tests if a digit is fount in cuv
int cuvant(char* cuv) {
    for (int i = 0; i < strlen(cuv); i++) {
        if (numar(cuv[i]) == 1)
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    FILE *f = stdin;
    
    TH* tabelaHash = NULL;
    tabelaHash = InitTH(codHash);   // initialize hashtable   
    char* command = calloc(10, sizeof(char));
    if (!command)
        return -1;

    int ok = 1;
    while (ok && fscanf(f ,"%s", command) != -1) {    // continously reads commands
        if (strcmp (command, "insert") == 0) {  // insert command
            char* line = NULL;
            line = calloc(1000, sizeof(char));
            if (!line)
                return 0;
            size_t len = 0;

            // getchar get rid of blank space after insert
            char c;
            fscanf(f, "%c", &c);     

            // reads every word after command
            fgets(line, 1000, f);

            // gets rid of newline character at the end of the line
            line[strcspn(line, "\n")] = '\0';   
            
            // divides the large string into individual words
            char* p = strtok(line, " ");
            while (p != NULL) {
                // tests if p does not contain digits
                if (cuvant(p) == 1) {
                    // gets rid of punctuation marks at the end of the word
                    if (p[strlen(p) - 1] == '.' || p[strlen(p) - 1] == ',')
                        p[strlen(p) - 1] = '\0';
                    if (strlen(p) >= 3) {
                        // inserts in hashtable
                        int rez = InsTH(tabelaHash, (void*) p, comparareLungime1);
                        if (!rez)
                            printf("Insert failed\n");
                    }
                }
                p = strtok(NULL, " ");
            }

            free(line);
            free(p);
        } else if (strcmp (command, "print") == 0) {
            // sorts words from sublists
            SortTH(tabelaHash);
            char c = 0;
            fscanf(f, "%c", &c);
            if (c != ' ') {        // print full hashtable
                AfiTH(tabelaHash, afi_elem);
            } else if (c == ' ') {
                char* printCommand = calloc(10, sizeof(char));
                if (!printCommand)
                    return -1;
                fscanf(f, "%s", printCommand);

                if (numar(printCommand[0]) == 1) {  // print n
                    // print only the words who occur maximum n times

                    int n = stringToInt(printCommand);
                    // function for print n
                    afisareMax(tabelaHash, n);

                } else {                            // print c n
                    // prints only the words where the first letter is c and appear exactly n times
                    int n;
                    fscanf(f, "%d", &n);
                    int cod = tabelaHash->fh((void*)printCommand);
                    if (tabelaHash->v[cod]) {
                        afisareCod(tabelaHash->v[cod], printCommand[0], n);
                    }
                }
                free(printCommand);
            }
        } else if (strcmp (command, "quit") == 0) {
            ok = 0;
        }
    }
    fclose(f);
    free(command);
    quit(&tabelaHash);
    return 0;
}
