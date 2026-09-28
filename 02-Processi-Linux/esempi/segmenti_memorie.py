# =============================================================================
# File: segmenti_memorie.py
# Argomento: Oggetti Python e confronto con i segmenti del C
# Scopo: Mostra gli identificatori di oggetti associati a nomi globali e locali.
# Esecuzione: python3 segmenti_memorie.py
# Nota: Gli scope Python non corrispondono direttamente ai segmenti .data, .bss e stack.
# =============================================================================
# --- SIMIL-SEGMENTO DATA/BSS (Scope Globale) ---
# In Python, le variabili globali devono essere inizializzate.
# Non esiste una variabile globale "non inizializzata".
#
# I nomi Python si riferiscono a oggetti gestiti dall’interprete. La loro
# visibilità (scope) non determina un segmento di memoria come avviene in C.

globale_inizializzata = 42

# None rappresenta qui l’assenza di un valore: è comunque un oggetto Python,
# non una variabile non inizializzata e non una collocazione nel segmento .bss.
globale_simil_bss = None  


def funzione(parametro_stack):
    # --- SIMIL-STACK (Scope Locale) ---
    # I nomi sono locali alla chiamata; gli oggetti possono sopravvivere se
    # conservano altri riferimenti. Un nome locale non è un oggetto C sullo stack.
    locale_stack = 10
    
    # global serve per riassegnare un nome globale dalla funzione. Per la sola
    # lettura fatta qui non sarebbe necessario; non crea una variabile statica C.
    global globale_inizializzata
    
    # id() identifica un oggetto per la sua durata di vita. In CPython coincide
    # con l’indirizzo, ma ciò non è garantito da tutte le implementazioni Python.
    # Oggetti condivisi (come alcuni piccoli interi) possono avere lo stesso id().
    print("--- INDIRIZZI DI MEMORIA IN PYTHON (id()) ---")
    print(f"Indirizzo oggetto globale_inizializzata: {id(globale_inizializzata)}")
    print(f"Indirizzo oggetto globale_simil_bss:     {id(globale_simil_bss)}")
    print(f"Indirizzo oggetto locale_stack:          {id(locale_stack)}")
    print(f"Indirizzo oggetto parametro_stack:       {id(parametro_stack)}")


# Esecuzione del codice
funzione(5)
