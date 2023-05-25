#include "login.h"

void load_login_data(hashtable_t *mm_users)
{
    char user[100];
    unsigned int pass;
    FILE *input = fopen("data/user_database.csv", "r");
    while(fscanf(input, "%[^~]~%u\n", user, &pass) == 2) {
        // printf("%s %u\n", user, pass);
        mm_put(mm_users, user, sizeof(user), &pass, sizeof(pass));
    }
    fclose(input);
}

linked_list_t *create_account(hashtable_t *mm_users, char *key, char *pass)
{
    unsigned int hashed_pass = hash_function_string(pass);
    mm_put(mm_users, key, strlen(key), pass, sizeof(pass));
    hashed_pass = mm_users->hash_function(key) % ht->hmax;
    return mm_users->buckets[hashed_pass];
    // pentru viitor, adauga userii noi in user_database
}
