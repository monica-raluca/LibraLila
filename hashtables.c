#include "hashtables.h"

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*))
{
	hashtable_t *ht = malloc(sizeof(hashtable_t));
	ht->size = 0;
	ht->hmax = hmax;
	ht->buckets = malloc(hmax * sizeof(linked_list_t *));
	for (int i = 0 ; i < hmax ; i++)
	    ht->buckets[i] = ll_create(sizeof(info));
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;
	ht->key_val_free_function = key_val_free_function;
	return ht;
}

int ht_has_key(hashtable_t *ht, void *key)
{
    unsigned int pos = ht->hash_function(key) % ht->hmax;
    if (ht->buckets[pos]->head != NULL) {
        ll_node_t *cursor = ht->buckets[pos]->head;
        while (cursor->next != NULL && (ht->compare_function(key, ((info *)cursor->data)->key)))
            cursor = cursor->next;
        if (!(ht->compare_function(key, ((info *)cursor->data)->key)))
            return 1;
    }
	return 0;
}

void *ht_get(hashtable_t *ht, void *key)
{
	unsigned int pos = ht->hash_function(key) % ht->hmax;
	if (ht->buckets[pos]->head != NULL) {
		ll_node_t *cursor = ht->buckets[pos]->head;
		while((cursor->next != NULL) && (ht->compare_function(key, ((info *)cursor->data)->key)))
			cursor = cursor->next;
		if (!(ht->compare_function(key, ((info *)cursor->data)->key)))
			return ((info *)cursor->data)->value;
	}
	return NULL;
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
    unsigned int pos = ht->hash_function(key) % ht->hmax;
    ll_node_t* cursor = ht->buckets[pos]->head;
    info* new_info = malloc(sizeof(info));

    new_info->key = malloc(key_size);
    memcpy(new_info->key, key, key_size);
    new_info->value = malloc(value_size);
    memcpy(new_info->value, value, value_size);

    if (ht_has_key(ht, key) == 0) {
        ll_add_nth_node(ht->buckets[pos], 0, new_info);
        ht->size++;
    } else {
		struct FriendInfo *current_info = ht_get(ht, key);
		free(current_info);
		current_info = malloc(value_size);
		memcpy(current_info, new_info->value, value_size);
    }
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
	unsigned int pos = ht->hash_function(key) % ht->hmax;
	if (ht_has_key(ht, key) == 0) {
		printf("No se puede\n");
		return;
	}
	ll_node_t *cursor = ht->buckets[pos]->head;
	int key_pos = 0;
	while (ht->compare_function(key, ((info *)cursor->data)->key)) {
		cursor = cursor->next;
		key_pos++;
	}
	key_val_free_function(cursor->data);
	ll_remove_nth_node(ht->buckets[pos], key_pos);
	ht->size--;
}
