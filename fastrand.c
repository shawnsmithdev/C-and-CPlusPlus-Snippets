#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const char * sizeNames[6] = { "B", "kB", "MB", "GB", "TB", "PB" };

void printResults(long long int bytes, double time) {
    double sBytes = bytes;
    double rate = bytes / time;
    int i = 0; // Name index for bytes
    int j = 0; // Name index for rate

    while (i < 6) {
        if (sBytes >= 1024) {
            sBytes /= 1024;
            i++;
 
            if (rate >= 1024) {
                rate /= 1024;
                j++;
            }

        } else {
            break;
        }
    }

    printf("Wrote %0.2f %s in %0.2fs (%0.2f %s/s)\n",
        sBytes, sizeNames[i], time, rate, sizeNames[j]
    );
}


int main(void) {
    const int buffSize = 1048576;
    const int intPerBuff = buffSize / sizeof(int);
    const int buffPerFile = 10240;

    unsigned int seed;
    int buffer[intPerBuff];
    int rnum;
    long long int bcount = 0;
    time_t start;
    time_t report;
    time_t end;

    FILE *urand = fopen("/dev/urandom", "r");
    FILE *ofile = fopen("/tmp/tmp.bin", "w");
    if (urand == NULL || ofile == NULL)
    {
        printf("Error opening files.\nurand: %p\nofile: %p\n", urand, ofile);
        return 0;
    }

    time(&start);
    time(&report);
    for (int i = 0; i < buffPerFile; i++)
    {
        fread(&seed, sizeof(unsigned int), 1, urand);
        for (int j = 0; j < intPerBuff; j++)
        {
            buffer[j] = rand_r(&seed);
        }
//        fwrite(buffer, sizeof(int), intPerBuff, ofile);
        bcount += sizeof(int) * intPerBuff;

        time(&end);
        if (difftime(end, report) > 1) {
            time(&report);
            printResults(bcount, difftime(end, start));
        }
    }
}
