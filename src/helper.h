#ifndef HERLPER_H
#define HERLPER_H

void printIntArray(int * a, int len) {
    printf("[");
    for(int i=0; i<len; i++) {
        printf("%d,", a[i]);
    }
    printf("]\n");
}

void printString(char * a, int len) {
    for(int i=0; i<len; i++) {
        printf("%c", a[i]);
    }
    printf("\n");
}

char *read_file(const char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "rb");

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, fp);
    fclose(fp);

    string[fsize] = '\0'; // terminate with zero
    return string;
}

#endif //HERLPER_H