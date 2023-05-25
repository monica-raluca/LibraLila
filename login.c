#include "login.h"

void login(hashtable_t *mm_users, linked_list_t **preferences, char *isbn, char *key, char *password)
{
    if (isbn[0] == 'Y') {
        while (!(*preferences)) {
            printf("New Username:");
            scanf("%s", key);
            printf("New Password:");
            scanf("%s", password);
            (*preferences) = create_account(mm_users, key, password);
            if ((*preferences)) {
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("\t\tWelcome %s!\t\t\n\n", key);
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                return;
            }
        }
        return;
    }
    else if (isbn[0] == 'N') {
        while (!(*preferences)) {
            printf("Username:");
            scanf("%s", key);
            printf("Password:");
            scanf("%s", password);
            (*preferences) = check_login_data(mm_users, key, password);
            if ((*preferences)) {
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("\t\tWelcome %s!\t\t\n\n", key);
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            }
            return;
        }
        return;
    } else {
        printf("Invalid reply\n");
        return;
    }
}

void load_login_data(hashtable_t *mm_users)
{
    char user[100];
    unsigned int pass;
    FILE *input = fopen("data/user_database.csv", "r");
    while(fscanf(input, "%[^~]~%u\n", user, &pass) == 2)
        mm_put(mm_users, user, sizeof(user), &pass, sizeof(pass));
    fclose(input);
}

linked_list_t *create_account(hashtable_t *mm_users, char *key, char *pass)
{
    unsigned int hashed_pass = hash_function_string(pass);
    if(ht_has_key(mm_users, key)) {
        printf("Username already taken! Please try again!\n");
        return NULL;
    }
    mm_put(mm_users, key, strlen(key), &hashed_pass, sizeof(hashed_pass));
    hashed_pass = mm_users->hash_function(key) % mm_users->hmax;
    FILE *database = fopen("data/user_database.csv", "a");
    fprintf(database, "%s~%d\n", key, hashed_pass);
    fclose(database);
    return ((info *)mm_users->buckets[hashed_pass]->head->data)->value;
}

linked_list_t *check_login_data(hashtable_t *mm_users, char *key, char *pass)
{
    unsigned int hashed_pass = hash_function_string(pass);
    info *user_info = ht_get(mm_users, key);
    if (!user_info) {
        printf("Inexistent user! Try again!\n");
        return NULL;
    } else if (*(unsigned int *)user_info->pass != hashed_pass) {
        printf("Wrong credentials! Try again!\n");
        return NULL;
    }
    return (linked_list_t *)user_info->value;
}

void add_preferences(hashtable_t *ht_books, linked_list_t *preferences, char *isbn)
{
    info *book_info = NULL;
    if (ht_has_key(ht_books, isbn)) {
        // if (list_has_key(preferences, isbn)) {
        //     printf("Book was already added\n");
        //     return;
        // }
        book_info = ht_get(ht_books, isbn);
        ll_add_nth_node(preferences, 0, book_info->value);
    } else {
        printf("ISBN is not associated with an existent book\n");
    }
}

void print_preferences(linked_list_t *preferences)
{
    ll_node_t *cursor = preferences->head;
    for (unsigned int i = 0; i < preferences->size; i++) {
        print_book((book *)cursor->data);
        cursor = cursor->next;
    }
}

void remove_preferences(linked_list_t *preferences, char *isbn)
{
    ll_node_t *cursor = preferences->head, *prev = NULL;
    for (unsigned i = 0; i < preferences->size; i++) {
        if (!strcmp(((book *)cursor->data)->isbn, isbn)) {
            printf("found\n");
            if (!prev)
                preferences->head = cursor->next;
            else
                prev->next = cursor->next;
            free(cursor->data);
            free(cursor);
            preferences->size--;
            break;
        }
        prev = cursor;
        cursor = cursor->next;
    }
}

// sort_by_author(linked_list_t *preferences)
// {
//     ll_node_t *cursor = preferences->head, *prev = NULL;
//     for (int i = 0; i < preferences->size - 1; i++) {
//         if (cursor->data->authors - cursor->next->data->authors > 0)
//             cursor->next = cursor->next->next;
//     }
// }

// void sort_by_date(linked_list_t *preferences)
// {
//     ll_node_t *cursor = preferences->head, *prev = NULL;
//     for (int i = 0; i < preferences->size - 1; i++)
//         if (cursor->data->published_year - cursor->data->published_year > 0)
//             if (!i)
//                 preferences->head = 
// }

ll_node_t *merge(ll_node_t *first, ll_node_t *second)
{
    ll_node_t *merged = calloc(1, sizeof(ll_node_t *));
    ll_node_t *temp = calloc(1, sizeof(ll_node_t *));

    memcpy(temp, merged, sizeof(ll_node_t));

    while(first && second) {
        if(strcmp(((book *)first->data)->authors, ((book *)second->data)->authors) <= 0) {
            temp->next = first;
            first = first->next;
        } else {
            temp->next = second;
            second = second->next;
        }
        temp = temp->next;
    }

    while(first) {
        temp->next = first;
        first = first->next;
        temp = temp->next;
    }

    while(second) {
        temp->next = second;
        second = second->next;
        temp = temp->next;
    }

    return merged->next;
}

ll_node_t *middle(ll_node_t *head)
{
    ll_node_t *slow = head;
    ll_node_t *fast = head->next;

    while (!slow->next && (!fast && !fast->next)) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

ll_node_t *merge_sort(ll_node_t *head)
{
    if (!head->next) 
        return head;
    ll_node_t *mid = calloc(1, sizeof(ll_node_t *));
    ll_node_t *second_head = calloc(1, sizeof(ll_node_t *));

    mid = middle(head);
    second_head = mid->next;
    mid->next = NULL;

    ll_node_t *final = merge(merge_sort(head), merge_sort(second_head));
    return final;
}

void sort_by_author(linked_list_t *preferences)
{
    // ll_node_t *cursor = preferences->head, *prev = NULL;
    // for (int i = 0; i < preferences->size - 1; i++) {
    //     if (cursor->data->authors - cursor->next->data->authors > 0)
    //         cursor->next = cursor->next->next;
    // }
    preferences->head = merge_sort(preferences->head);
}

void sort_preferences(linked_list_t *preferences, int criteria)
{
    if (criteria == 0) {
        sort_by_author(preferences);
    } else if (criteria == 1) {
        // sort_by_title(linked_list_t preferences);
    } else if (criteria == 2) {
        // sort_by_genre(linked_list_t preferences);
    } else if (criteria == 3) {
        // sort_by_date(linked_list_t preferences);
    } else if (criteria == 4) {
        // sort_by_rating(linked_list_t preferences);
    } else {
        printf("Invalid criteria\n");
        return;
    }
}
