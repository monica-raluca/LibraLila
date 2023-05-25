#include "login.h"

void print_book(book *desired_book);
void load_csv(hashtable_t *ht_books);
void print_isbn(hashtable_t *ht_books, char *isbn);
void print_description(hashtable_t *ht_books, char *isbn);
void change_description(hashtable_t *ht_books, char *isbn);
void rate_book(hashtable_t *ht_books, char *isbn);
void show_books(hashtable_t *ht_books, int criteria);
void show_author(hashtable_t *ht_books);