#include "hashtables.h"

/*
 * Procedura pentru incarcarea fisierului/fisierelor cu date de conectare si
 * initializarea listelor de preferinte ale utilizatorilor.
 */
void load_login_data(hashtable_t *mm_users);

/*
 * Functie care primeste ca parametru o cheie (parola) si o valoare (username)
 * si verifica daca exista o intrare in hashtables asociata acestora.
 * In cazul in care exista, va fi returnata lista de preferinte asociata
 * utilizatorului. In caz contrar, va fi returnat pointer-ul NULL si va fi
 * afisat un mesaj de eroare.
 */
linked_list_t *check_login_data(hashtable_t *mm_users, char *key, char *pass);

/*
 * Functie care primeste ca parametrii o cheie (parola) si o valoare (username)
 * Daca exista deja o intrare asociata acelui username se va afisa un mesaj de
 * eroare si va fi returnata pointerul NULL. In caz contrar se va crea un
 * utilizator si i se va returna lista de preferinte asociata.
 */
linked_list_t *create_account(hashtable_t *mm_users, char *key, char *pass);

/*
 * Procedura care primeste ca parametru lista de preferinte a unui utilizator
 * si ii afiseaza cartile adaugate in lista de preferinte.
 */
void print_preferences(linked_list_t *preferences);

/*
 * Procedura ce primeste ca parametru lista de preferinte a unui utilizator si
 * un pointer catre datele despre cartea dorita si o adauga in lista sa.
 */
void add_preferences(hashtable_t *ht_books, linked_list_t *preferences, char *isbn);

/*
 * Procedura ce primeste ca parametru lista de preferinte a utilizatorului si
 * isbn-ul unei carti pe care doreste sa o adauge in lista de preferinte
 */
void remove_preferences(linked_list_t *preferences, char *isbn);
