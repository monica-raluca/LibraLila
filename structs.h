#include "utils.h"

typedef struct ll_node_t
{
	void* data;
	struct ll_node_t* next;
} ll_node_t;

typedef struct linked_list_t
{
	ll_node_t* head;
	unsigned int data_size;
	unsigned int size;
} linked_list_t;

typedef struct book {
    int published_year, page_count, author_count, rating_count;
    float rating;
    char isbn[11], title[MAX_STRING_SIZE], authors[MAX_STRING_SIZE], genre[MAX_STRING_SIZE], description[MAX_STRING_SIZE];
} book;

typedef struct info info;
struct info {
	void *key; // usernames
	void *value; // preference list
	void *pass; // hashed password
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
	void (*key_val_free_function)(void*);
};