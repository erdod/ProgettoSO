# Progetto: Simulatore di Sistema di Memoria Virtuale

## Descrizione del Progetto

Il progetto consiste in un simulatore di sistema di memoria virtuale. Viene implementato un gestore di memoria virtuale (MMU) e un gestore di memoria fisica per gestire la mappatura delle pagine virtuali nella memoria fisica. Il sistema è testato attraverso l'utilizzo di un eseguibile di prova che effettua scritture e letture casuali dalla memoria virtuale.

## File Utilizzati

1. **mmu.h**: Header file contenente le dichiarazioni delle funzioni per la gestione dell'unità di gestione della memoria (MMU).

2. **mmu.c**: File di implementazione delle funzioni dichiarate in `mmu.h`. Qui sono definite le funzioni per l'inizializzazione e la distruzione dell'MMU, la scrittura e la lettura dei byte dalla memoria virtuale e la gestione delle eccezioni di page fault.

3. **physical_memory_mng.h**: Header file contenente le dichiarazioni delle funzioni per la gestione della memoria fisica.

4. **physical_memory_mng.c**: File di implementazione delle funzioni dichiarate in `physical_memory_mng.h`. Qui sono definite le funzioni per l'inizializzazione e la distruzione della memoria fisica, nonché per la riserva e il rilascio dei frame fisici.

5. **test.c**: File di test contenente la funzione principale `main()`. Qui vengono eseguite le operazioni di scrittura e lettura dalla memoria virtuale e vengono stampati i risultati.

6. **swap_file.bin**: Il file `swap_file.bin` è utilizzato come area di scambio per memorizzare le pagine della memoria virtuale che sono state scambiate con la memoria fisica a causa di page faults durante l'esecuzione del simulatore di memoria virtuale.


## Simulazione

La simulazione avviene attraverso l'eseguibile `test`, che sfrutta le funzioni definite nei file sopra elencati. La simulazione procede nel seguente modo:

1. **Inizializzazione dell'MMU**: Viene allocata e inizializzata la memoria virtuale.

2. **Inizializzazione della memoria fisica**: Viene allocata e inizializzata la memoria fisica, suddivisa in frame.

3. **Scrittura nella memoria virtuale**: Vengono effettuate scritture casuali nella memoria virtuale.

4. **Lettura dalla memoria virtuale**: Vengono effettuate letture casuali dalla memoria virtuale e confrontati i valori letti con quelli scritti precedentemente.

5. **Stampa dei risultati**: Vengono stampati i tempi impiegati per le operazioni di scrittura e lettura, nonché il numero di errori riscontrati durante la lettura.

## Risultati

L'eseguibile di test restituisce i seguenti risultati:

- **Tempo per la scrittura dei dati**: Tempo impiegato per scrivere dati casuali nella memoria virtuale.
- **Tempo per la lettura dei dati (accesso sequenziale)**: Tempo impiegato per leggere i dati dalla memoria virtuale in modo sequenziale.
- **Tempo per la lettura dei dati (accesso casuale)**: Tempo impiegato per leggere i dati dalla memoria virtuale in modo casuale.
- **Errori**: Numero totale di errori riscontrati durante la lettura dei dati dalla memoria virtuale.

I risultati possono variare in base all'efficienza dell'implementazione dell'MMU e del gestore della memoria fisica.
