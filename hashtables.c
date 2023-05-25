#include "hashtables.h"

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

void ht_key_val_free_function(void *data) {
    free(((info*)data)->key);
    free(((info*)data)->value);
}

void mm_key_val_free_function(void *data) {
    free(((info*)data)->key);
    for(unsigned int i = 0; 
    i < (((linked_list_t*)((info*)data)->value)->size); i++)
        ll_free(((((info*)data)->value)));
    free(((info*)data)->value);
}

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*))
{
    hashtable_t *ht = calloc(1, sizeof(hashtable_t));
    ht->hmax = hmax;
    ht->size = 0;
    ht->buckets = calloc(ht->hmax, sizeof(linked_list_t*));
    for(unsigned int i = 0; i < ht->hmax; i++)
        ht->buckets[i] = ll_create(sizeof(info));
    ht->hash_function = hash_function;
    ht->compare_function = compare_function;
    ht->key_val_free_function = key_val_free_function;
    return ht;
}

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

void mm_put(hashtable_t *ht, void *key, unsigned int key_size, void *pass, unsigned int pass_size)
{
    unsigned int index = ht->hash_function(key) % ht->hmax;
    linked_list_t* bucket = ht->buckets[index];
    ll_node_t* node = bucket->head;
    info* new_info = malloc(sizeof(info));

    new_info->key = malloc(key_size);
    memcpy(new_info->key, key, key_size);

    new_info->pass = malloc(pass_size);
    memcpy(new_info->pass, pass, pass_size);

    if(ht_has_key(ht, key) == 0)
    {
        new_info->value = ll_create(sizeof(book));
        // void *added_info = malloc(value_size);
        // memcpy(added_info, value, value_size);
        // ll_add_nth_node(new_info->value, 0, added_info);
        ll_add_nth_node(bucket, 0, new_info);
        ht->size++;
    }
    else
    {
        // while(node)
        // {
        //     info* current_info = (info*) node->data;
        //     if(ht->compare_function(key, current_info->key) == 0)
        //     {
        //         void *added_info = malloc(value_size);
        //         memcpy(added_info, value, value_size);
        //         ll_add_nth_node(current_info->value, 
        //         ((linked_list_t*)current_info->value)->size, added_info);
        //     }
        //     node = node->next;
        // }
        // Prints error message
        printf("Username is already taken!\n");
        return;
    }
}

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
            ht_key_val_free_function(current_info);
            ll_remove_nth_node(ht->buckets[index], count);
            break;
        }
        count++;
        node = node->next;
    }
    ht->size--;
}

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

void mm_free(hashtable_t *ht)
{
    for(unsigned int i = 0; i < ht->hmax; i++)
    {
        linked_list_t* list = ht->buckets[i];
        ll_node_t* temp = list->head;
        for(unsigned int j = 0; j < list->size; j++)
            ll_free(((info*)temp->data)->value);
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
