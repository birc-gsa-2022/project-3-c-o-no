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
    for(int i=0; i<fasta.alphabet.size; i++) {
        int sighting = bucketsIndecies[i];
        bucketsIndecies[i] = accumSum;
        accumSum += sighting;
    }
    int* buckets = malloc(fasta.alphabet.size*sizeof *buckets);

    int charIndex;
    char c;
    int elemInBucket;
    int saIndex;
    for(int i=n-1; i>=0; i--) {
        for(int j=0; j<fasta.alphabet.size; j++) buckets[j] = 0;
        for(int j=0; j<n; j++) {
            charIndex = ((*currentSa)[j] + i) % n;
            c = x[charIndex];
            elemInBucket = buckets[c]++;
            saIndex = bucketsIndecies[c] + elemInBucket;
            (*otherSa)[saIndex] = (*currentSa)[j];
        }
        temp = currentSa;
        currentSa = otherSa;
        otherSa = temp;
    }
    return *currentSa;
}

struct Interval {
    int start;
    int mid;
    int end;
};

struct Interval binarySearch(const char* x, const int* sa, char patchar, int parIndex, struct Interval interval, int mode) {
    char xchar;
    while (interval.start != interval.end) {
        interval.mid = (interval.start+interval.end)/2;
        xchar = x[sa[interval.mid]+parIndex];
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

struct Interval searchPatternInSA(struct Fasta fasta, const char* pattern, int* sa, int m) {
    char * x = fasta.fasta_sequence;
    struct Interval * interval = malloc(sizeof *interval);
    struct Interval * intervalSaver = malloc(sizeof *interval);
    interval->start = 0;
    interval->end = fasta.fasta_len;

    for(int i=0; i<m; i++) {
        char patchar = fasta.alphabet.symbols[pattern[i]];
        *interval = binarySearch(x, sa, patchar, i, *interval, 0);
        if(interval->start == interval->end) return *interval;
        intervalSaver->mid = interval->mid;
        intervalSaver->end = interval->end;

        interval->end = interval->mid;
        *interval = binarySearch(x, sa, patchar, i, *interval, -1);
        intervalSaver->start = interval->start;

        interval->start = intervalSaver->mid;
        interval->end = intervalSaver->end;

        *interval = binarySearch(x, sa, patchar, i, *interval, 1);
        interval->start = intervalSaver->start;
    }
    return *interval;
}