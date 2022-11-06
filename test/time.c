#include <stdio.h>
#include <malloc.h>
#include "../src/parsers/simple-fasta-parser.h"
#include "../src/helper.h"
#include "../src/sa.h"
#include <sys/time.h>
#include <math.h>
#define maxN 1000000
#define minN 100000
#define stepN 100000
#define minM 100000
#define stepM 100000
#define description "sameChar"


struct testData {
    char * x;
    char * pattern;
    int n;
    int m;
};


char * getPattern(int m, char* p) {
    for(int i=0; i<m; i++) {
        p[i] = 'a';
    }
    p[m] = '\0';
    return p;
}

//Does not work, instead we read from file
char * getX(int n, char *x) {
    x[0] = '>';
    x[1] = ' ';
    x[2] = 'a';
    x[3] = '\n';
    for(int i=0; i<n; i++) {
        x[i+4] = 'a';
    }
    x[n+4] = '\n';
    x[n+5] = '\0';
    return x;
}

struct testData* getTest(FILE *file, int n, int m, char *x, char *p) {
    struct testData* data = malloc(sizeof *data);
    data->n = n;
    data->m = m;
    data->pattern = getPattern(m, p);
    data->x = getX(n, x);

    /*
    int nLength = (int)log10(n)+2;
    char fileName[28+nLength];
    sprintf(fileName, "test-data/sameChar/fasta-%d.fa", n);
    data->x = read_file(fileName);*/

    return data;
}

FILE *getFile() {
    FILE *fpt;
    fpt = fopen("timingBigger.csv", "w+");
    return fpt;
}

int getTime(struct timeval start, struct timeval stop) {
    // Source: https://stackoverflow.com/questions/10192903/time-in-milliseconds-in-c
    return (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}

struct FastaContainer * timeParseFasta(FILE* file, char *fasta_str) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    struct FastaContainer *con = parse_fasta(fasta_str);

    gettimeofday(&stop, NULL);
    fprintf(file, "%d,", getTime(start, stop));

    return con;
}

int * timeConstructSARadix(FILE* file, struct Fasta fasta) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    int* sa = constructSARadix(fasta);

    gettimeofday(&stop, NULL);
    fprintf(file, "%d,", getTime(start, stop));

    return sa;
}

struct Interval timeSearch(FILE* file, struct Fasta fasta, char* pattern, int* sa, int m) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    struct Interval interval = searchPatternInSA(fasta, pattern, sa, m);

    gettimeofday(&stop, NULL);
    fprintf(file, "%d,", getTime(start, stop));

    return interval;
}


int main() {
    FILE *file = getFile();
    fprintf(file,"n,m,parseTime,SATime,SearchTime,Description\n");

    char *x = malloc((sizeof *x)*(maxN + 6));
    char *pattern = malloc((sizeof *pattern)*(maxN + 1));

    for(int n=minN; n<maxN+1; n+=stepN) {
        int m = n;
        //for(int m=minM; m<n; m+=stepM) {
            struct testData *data = getTest(file, n, m, x, pattern);
            fprintf(file, "%d,%d,", n, m);
            struct FastaContainer *con = timeParseFasta(file, data->x);
            int *sa = timeConstructSARadix(file, **(con->fastas));
            timeSearch(file, **(con->fastas), data->pattern, sa, m);
            fprintf(file, "%s\n", description);

            free(data);
            free(con);
            free(sa);
        //}
    }
}