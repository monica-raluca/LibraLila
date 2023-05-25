#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtables.h"

/*
 * Functii de comparare a cheilor:
 */
int compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/*
 * Functii de hashing:
 */
unsigned int hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*
 * Functie apelata pentru a elibera memoria ocupata de cheia si valoarea unei
 * perechi din hashtable. Daca cheia sau valoarea contin tipuri de date complexe
 * aveti grija sa eliberati memoria luand in considerare acest aspect.
 */
void key_val_free_function(void *data) {
    free(((info*)data)->key);
    free(((info*)data)->value);
}

/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*))
{
    hashtable_t *ht = calloc(1, sizeof(hashtable_t));
    ht->hmax = hmax;
    ht->size = 0;
    ht->buckets = calloc(ht->hmax, sizeof(linked_list_t*));
    for(int i = 0; i < ht->hmax; i++)
        ht->buckets[i] = ll_create(sizeof(info));
    ht->hash_function = hash_function;
    ht->compare_function = compare_function;
    ht->key_val_free_function = key_val_free_function;
    return ht;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable
 * folosind functia put;
 * 0, altfel.
 */
int ht_has_key(hashtable_t *ht, void *key)
{
    int index = ht->hash_function(key) % ht->hmax;
    linked_list_t* bucket = ht->buckets[index];
    ll_node_t* node = bucket->head;
    while(node)
    {
        info* current_info = (info*) node->data;
        if(ht->compare_function(key, current_info->key) == 0)
            return 1;
        node = node->next;
    }
	return 0;
}

void *ht_get(hashtable_t *ht, void *key)
{
    int index = ht->hash_function(key) % ht->hmax;
    linked_list_t* bucket = ht->buckets[index];
    ll_node_t* node = bucket->head;
    while(node)
    {
        info* current_info = (info*) node->data;
        if(ht->compare_function(key, current_info->key) == 0)
            return current_info->value;
        node = node->next;
    }
    return NULL;
}

/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune
 * tipul ei), in momentul in care se creeaza o noua intrare in hashtable (in
 * cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie a
 * valorii la care pointeaza key si adresa acestei copii trebuie salvata in
 * structura info asociata intrarii din ht. Pentru a sti cati octeti trebuie
 * alocati si copiati, folositi parametrul key_size.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel
 * put(ht, key_actual, value_actual), valoarea la care pointeaza key_actual
 * poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia
 * unei intrari din hashtable. Nu ne dorim acest lucru, fiindca exista riscul sa
 * ajungem in situatia in care nu mai stim la ce cheie este inregistrata o
 * anumita valoare.
 */
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
    unsigned int index = ht->hash_function(key) % ht->hmax;
    linked_list_t* bucket = ht->buckets[index];
    ll_node_t* node = bucket->head;
    info* new_info = malloc(sizeof(info));

    new_info->key = malloc(key_size);
    memcpy(new_info->key, key, key_size);
    new_info->value = malloc(value_size);
    memcpy(new_info->value, value, value_size);

    if(ht_has_key(ht, key) == 0)
    {
        ll_add_nth_node(bucket, 0, new_info);
        ht->size++;
    }
    else
    {
        while(node)
        {
            info* current_info = (info*) node->data;
            if(ht->compare_function(key, current_info->key) == 0)
            {
                free(current_info->value);
                current_info->value = malloc(value_size);
                memcpy(current_info->value, new_info->value, value_size);
            }
            node = node->next;
        }
    }
}

/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o
 * intrare din hashtable (adica memoria pentru copia lui key --vezi observatia
 * de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void ht_remove_entry(hashtable_t *ht, void *key)
{
    int index = ht->hash_function(key) % ht->hmax;
    ll_node_t* node = ht->buckets[index]->head;
    int count = 0;
    while(node)
    {
        info* current_info = (info*) node->data;
        if(ht->compare_function(key, current_info->key) == 0)
        {
            key_val_free_function(current_info);
            ll_remove_nth_node(ht->buckets[index], count);
            break;
        }
        count++;
        node = node->next;
    }
    ht->size--;
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable,
 * dupa care elibereaza si memoria folosita pentru a stoca structura hashtable.
 */
void ht_free(hashtable_t *ht)
{	
    for(int i = 0; i < ht->hmax; i++)
    {
        linked_list_t* list = ht->buckets[i];
        ll_free(&list);
    }
    free(ht->buckets);
    free(ht);
}

unsigned int ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
