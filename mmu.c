#include "mmu.h"
#include "physical_memory_mng.h"

// Inizializzazione dell'MMU
MMU *initializeMMU() {
	MMU *mmu = (MMU *)malloc(sizeof(MMU));
	mmu->physical_memory = (char *)calloc(PHYSICAL_MEMORY_SIZE, sizeof(char));
	__init_phisical_memory(PAGE_SIZE, PHYSICAL_MEMORY_SIZE);

	// Inizializza la tabella delle pagine
	mmu->page_table = (PageTableEntry *)calloc(PAGE_TABLE_ENTRIES, sizeof(PageTableEntry));
	for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		mmu->page_table[i].flags = 0;
		mmu->page_table[i].physical_page_index = -1;
	}

	// Apre il file di swap
	mmu->swap_file = fopen("swap_file.bin", "w+b");
	if (mmu->swap_file == NULL) {
		printf("Impossibile aprire il file di swap.\n");
		exit(1);
	}
	return mmu;
}

// Deallocazione dell'MMU
void destroyMMU(MMU *mmu) {
	fclose(mmu->swap_file);
	free(mmu->physical_memory);
	free(mmu->page_table);
}

// Scrive un singolo byte, accedendo alla memoria nella posizione pos
void MMU_writeByte(MMU *mmu, int pos, char c) {
	int page = pos / PAGE_SIZE;
	int offset = pos % PAGE_SIZE;

	if (!(IS_VALID(mmu->page_table[page].flags)))
		pageFaultExcepHandler(mmu, pos);

	mmu->page_table[page].flags |= FLAG_WRITE_BIT;
	mmu->page_table[page].flags |= FLAG_UNSWAPPABLE;
	mmu->physical_memory[mmu->page_table[page].physical_page_index * PAGE_SIZE + offset] = c;
}

// Legge un byte dalla memoria virtuale
char MMU_readByte(MMU *mmu, int pos) {
	int page = pos / PAGE_SIZE;
	int offset = pos % PAGE_SIZE;

	if (!(IS_VALID(mmu->page_table[page].flags)))
		pageFaultExcepHandler(mmu, pos);

	mmu->page_table[page].flags |= FLAG_READ_BIT;
	mmu->page_table[page].flags |= FLAG_UNSWAPPABLE;
	return mmu->physical_memory[mmu->page_table[page].physical_page_index * PAGE_SIZE + offset];
}

// Seleziona la pagina da sostituire quando non c'è spazio sufficiente in memoria fisica
int electPageToSwapOut(MMU *mmu) {
	for (int i = 0; i < PAGE_TABLE_ENTRIES * 2; i++) {
		if (IS_VALID(mmu->page_table[i % (PAGE_TABLE_ENTRIES)].flags)) {
			if (IS_UNSWAPPABLE(mmu->page_table[i % PAGE_TABLE_ENTRIES].flags)) {
				// Seconda possibilità, resetta i bit FLAG_UNSWAPPABLE e continua
				mmu->page_table[i].flags &= ~FLAG_UNSWAPPABLE;
			}
			else {
				// Trovata una pagina vittima da sostituire. È la prima pagina SWAPPABLE.
				return i % (PAGE_TABLE_ENTRIES);
			}
		}
	}
	return -1;
}

// Gestisce l'eccezione di page fault
void pageFaultExcepHandler(MMU *mmu, int pos) {
	// Calcola il numero della pagina
	int page = pos / PAGE_SIZE;
	int page_to_swap = -1;

	int next_frame = __reserve_frame();
	if (next_frame < 0) {
		// Non è disponibile un frame fisico per la pagina.
		// Dobbiamo liberare un frame!
		page_to_swap = electPageToSwapOut(mmu);

		if (page_to_swap >= 0) {
			// Libera il frame collegato alla pagina da sostituire
			if (IS_WRITE(mmu->page_table[page_to_swap].flags)) {
				// Scrive nel file di swap solo se la pagina è stata scritta
				fseek(mmu->swap_file, page_to_swap * PAGE_SIZE, SEEK_SET);
				fwrite(&mmu->physical_memory[mmu->page_table[page_to_swap].physical_page_index * PAGE_SIZE], sizeof(char), PAGE_SIZE, mmu->swap_file);
			}
			__release_frame(mmu->page_table[page_to_swap].physical_page_index);
			mmu->page_table[page_to_swap].flags = 0;
		}
		else {
			printf("Pagina da sostituire non trovata. Uscita dal programma!\n");
			exit(1);
		}
		next_frame = __reserve_frame();
	}

	if (next_frame >= 0) {
		mmu->page_table[page].physical_page_index = next_frame;
		mmu->page_table[page].flags = FLAG_VALID | FLAG_UNSWAPPABLE;
		// Effettua lo swap della pagina richiesta
		fseek(mmu->swap_file, page * PAGE_SIZE, SEEK_SET);
		fread(&mmu->physical_memory[mmu->page_table[page].physical_page_index * PAGE_SIZE], sizeof(char), PAGE_SIZE, mmu->swap_file);
	}
	else {
		printf("Impossibile effettuare lo swap di una pagina. Uscita dal programma!\n");
		exit(1);
	}
}
