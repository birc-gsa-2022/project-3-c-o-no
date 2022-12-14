#ifndef SIMPLE_FASTA_PARSER_H
#define SIMPLE_FASTA_PARSER_H
#include <ctype.h>
#include <malloc.h>

#define LPA 128 // Last printable ascii
#define FPA 32 // First printable ascii

int magic_number = 8000;



struct Alphabet {
    int size;
    int *symbols; //TODO delete
    int *sightings;
};

struct Fasta {
    char *fasta_head;
    char *fasta_sequence;
    char *fasta_sequence_debugger; //TODO remove
    int fasta_len;
    struct Alphabet alphabet;
};

int *alloc_sightings(int *bigAlphabet, int alphabetSize) {
    int *sight = malloc(alphabetSize * sizeof *sight);
    sight[0] = 1;
    int j = 1;
    for(int k=FPA; k<LPA; k++) {
        if(bigAlphabet[k]) {
            sight[j] = bigAlphabet[k];
            bigAlphabet[k] = j++;
        }
    }
    return sight;
}


char *read_fasta_head(char **strptr) {
    char *string = *strptr;
    // We assume first character is '>', so skip it
    string++;
    // trim leading spaces
    while (isspace(*string)) {
        string++;
    }
    // Our header starts from this point
    char* header_start = string;
    // Move forward until a newline is encountered
    int i = 0;
    while (string[i] != '\n') {
        i++;
    }

    // Go back until first non-space character and thus trim trailing spaces and the \n
    int j = i;
    while (isspace(header_start[j])) header_start[j--] = '\0';
    // Move the pointer of the string to start of header plus count of all characters in header plus 1
    *strptr = header_start+i+1;
    // Now the start of the string points to the first sequence character, and we can return the FASTA header
    return header_start;
}

void update_fasta_by_sequence(char **strptr, struct Fasta *f) {
    // Space for all printable ascii chars, where each entry corresponds to how many times we've seen a character
    int *bigAlphabet = calloc(LPA, sizeof(int));
    // alphabetSize is for example 5 for ACGT (+1 for sentinel)
    int alphabetSize = 1;

    char *string = *strptr;
    int i = 0;
    int shift = 0;
    while (1) {
        // Increase shift for each \n we encounter
        while (string[i+shift] == '\n' || string[i+shift] == '\r') {
            shift++;
        }
        // If we encounter a \0 or >, then we are done
        if (string[i+shift] == '\0' || string[i+shift] == '>') {
            break;
        }
        // Look ahead and store c
        char c = string[i+shift];
        string[i] = c; // Apply the shift
        if(!(bigAlphabet[c]++)) alphabetSize++; // If first time seen symbol, add to size
        i++;
    }
    // Terminate with \0 and move the pointer to the first instance of >
    string[i] = 0;
    *strptr += i+shift;
    int *sightings = alloc_sightings(bigAlphabet, alphabetSize);

    char * debugger = malloc((i+1)*sizeof *debugger);
    debugger[i+1] = '\0';

    for(int l=0; l<i+1; l++) {
        debugger[l] = string[l];
        string[l] = (char) bigAlphabet[string[l]];
    }

    f->alphabet.size = alphabetSize;
    f->alphabet.symbols = bigAlphabet;
    f->alphabet.sightings = sightings;
    f->fasta_sequence = string;
    f->fasta_len = i+1;
    f->fasta_sequence_debugger = debugger;
}

struct FastaContainer {
    struct Fasta ** fastas;
    int numberOfFastas;
};

struct FastaContainer *parse_fasta(char *fasta_str) {
    struct Fasta **fastas = malloc(magic_number*sizeof (**fastas));
    int i = 0;
    while (fasta_str[0] != '\0') {
        char *header = read_fasta_head(&fasta_str);
        struct Fasta *f = malloc(sizeof(*f));
        f->fasta_head = header;
        update_fasta_by_sequence(&fasta_str, f);
        fastas[i++] = f;
    }
    struct FastaContainer *fastaCont = malloc(sizeof *fastaCont);
    fastaCont->fastas = fastas;
    fastaCont->numberOfFastas = i;

    return fastaCont;
}

void free_fastas(struct Fasta **fastas, int count) {
    for (int i = 0; i < count; ++i) {
        free(fastas[i]->alphabet.symbols);
        free(fastas[i]->alphabet.sightings);
        free(fastas[i]);
    }
}

void free_fasta_container(struct FastaContainer *fc) {
    // Free all the fastas
    free_fastas(fc->fastas, fc->numberOfFastas);
    // Free pointers to the fastas
    free(fc->fastas);
    // Finally, free the container
    free(fc);
}

#endif //SIMPLE_FASTA_PARSER_H
