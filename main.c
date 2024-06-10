#include "mmu.h"
#include <time.h>
#include <stdlib.h>

// Test della memoria

int main()
{
    MMU *mmu;
    char c;
    int missCount = 0;
    int countRandRead = 100000;
    clock_t timeForWrite, timeForSequentialRead, timeForRandomRead;
    char *buffer = calloc(VIRTUAL_MEMORY_SIZE, sizeof(char));

    mmu = initializeMMU();

    // Accesso alla memoria virtuale
    timeForWrite = clock();
    for (int i = 0; i < VIRTUAL_MEMORY_SIZE; i++) {
        c = rand() % 256;
        buffer[i] = c;
        MMU_writeByte(mmu, i, c);
    }
    timeForWrite = clock() - timeForWrite;

    // Lettura dalla memoria virtuale in modo sequenziale
    timeForSequentialRead = clock();
    for (int i = 0; i < VIRTUAL_MEMORY_SIZE; i++) {
        char r = MMU_readByte(mmu, i);
        if (r != buffer[i]) {
            missCount++;
        }
    }
    timeForSequentialRead = clock() - timeForSequentialRead;

    // Lettura casuale dalla memoria virtuale
    timeForRandomRead = clock();
    for (int i = 0; i < countRandRead; i++) {
        int addr = rand() % VIRTUAL_MEMORY_SIZE;
        char r = MMU_readByte(mmu, addr);
        if (r != buffer[addr]) {
            missCount++;
        }
    }
    timeForRandomRead = clock() - timeForRandomRead;

    printf("-------------------------------------------\n");
    printf("Milliseconds per scrivere i dati: %lf\n", (double)timeForWrite / (double)CLOCKS_PER_SEC * 1000);
    printf("Milliseconds per leggere i dati (%d accessi sequenziali): %lf\n", VIRTUAL_MEMORY_SIZE, (double)timeForSequentialRead / (double)CLOCKS_PER_SEC * 1000);
    printf("Milliseconds per leggere i dati (%d accessi casuali): %lf\n", countRandRead, (double)timeForRandomRead / (double)CLOCKS_PER_SEC * 1000);
    printf("Errori: %d\n", missCount);
    printf("-------------------------------------------\n");

    destroyMMU(mmu);
    free(buffer);
    return 0;
}
