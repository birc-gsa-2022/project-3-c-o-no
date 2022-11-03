#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include <stdint-gcc.h>
#include "helper.h"

int * constructSARadix(struct Fasta fasta) {
    char* x = fasta.fasta_sequence;
    int n = fasta.fasta_len;
    int* sa = malloc(n * sizeof *sa);
    int* saCopy = malloc(n * sizeof *sa);
    int** currentSa = &sa;
    int** otherSa = &saCopy;
    int** temp;

    for (int i = 0; i<n; i++) {
        sa[i] = i;
    }
    int* bucketsIndecies = fasta.alphabet.sightings;
    int accumSum = 0;
    for(int i=0; i<fasta.alphabet.size; i++) { //TODO invariant will break when multiple fastas. Make into multiple functions
        int sighting = bucketsIndecies[i];
        bucketsIndecies[i] = accumSum;
        accumSum += sighting;
    }
    int* buckets = malloc(fasta.alphabet.size*sizeof *buckets);

    for(int i=n-1; i>=0; i--) {
        for(int j=0; j<fasta.alphabet.size; j++) buckets[j] = 0;
        for(int j=0; j<n; j++) {
            //TODO move initilization outside loop
            int charIndex = ((*currentSa)[j] + i) % n;
            char c = x[charIndex];
            int elemInBucket = buckets[c]++;
            int saIndex = bucketsIndecies[c] + elemInBucket;
            (*otherSa)[saIndex] = (*currentSa)[j];
        }
        temp = currentSa;
        currentSa = otherSa;
        otherSa = temp;
    }

    return sa;
}

struct Interval {
    int start;
    int mid;
    int end;
};

struct Interval binarySearch(char* x, int* sa, char patchar, int parIndex, struct Interval interval, int mode) {
    char xchar;
    while (interval.start != interval.end) {
        interval.mid = (interval.start+interval.end)/2;
        xchar = x[sa[interval.mid]] + parIndex;
        if(patchar == xchar) {
            if(!mode) return interval;
            if(mode==1) interval.start = interval.mid + 1;
            else interval.end = interval.mid;
        }
        else {
            if (xchar > patchar) {
                interval.end = interval.mid;
            }
            else interval.start = interval.mid + 1;
        }
    }
    return interval;
}

struct Interval searchPattenInSA(char* x, const char* pattern, int* sa, int n, int m) {
    struct Interval * interval = malloc(sizeof *interval);
    struct Interval * intervalSaver = malloc(sizeof *interval);
    interval->start = 0;
    interval->end = n;

    for(int i=0; i<m; i++) {
        char patchar = pattern[i];
        binarySearch(x, sa, patchar, i, *interval, 0);
        if(interval->start == interval->end) return *interval;
        intervalSaver->mid = interval->mid;
        intervalSaver->end = interval->end;

        interval->end = interval->mid;
        binarySearch(x, sa, patchar, i, *interval, -1);
        intervalSaver->start = interval->start;

        interval->start = intervalSaver->mid;
        interval->end = intervalSaver->end;

        binarySearch(x, sa, patchar, i, *interval, 1);
    }

    interval->start = intervalSaver->start;
    return *interval;
}