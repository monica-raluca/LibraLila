#include "books_management.h"

void show_instructions()
{
    FILE *fhelp = fopen("data/help.txt", "r");
    char buffer[MAX_STRING_SIZE];
    for (int i = 1; i <= 19; i++) {
        fgets(buffer, MAX_STRING_SIZE, fhelp);
        puts(buffer);
    }
    fclose(fhelp);
}

int main(void)
{
    char *key, *op, *password, isbn[11];
    int criteria;
    password = calloc(1, MAX_STRING_SIZE);
    key = calloc(1, MAX_STRING_SIZE);
    op = calloc(1, MAX_STRING_SIZE);
    hashtable_t *mm_users = ht_create(HMAX, hash_function_string, compare_function_strings, mm_key_val_free_function);
    hashtable_t *ht_books = ht_create(HMAX, hash_function_string, compare_function_strings, ht_key_val_free_function);

    DIE(!ht_books || !mm_users, "Hashtable memory allocation error");
    DIE(!key || !op || !password, "Unable to allocate string memory");
    
    load_login_data(mm_users);
    load_csv(ht_books);

    // print_isbn(ht_books, isbn);
    
    linked_list_t *preferences = NULL;
    while (1) {
        printf("Do you have an account? [Y/N]\n");
        scanf("%s", isbn);
        if (isbn[0] == 'Y')
            break;
        else if (isbn[0] == 'N') {
            printf("New Username:");
            scanf("%s", key);
            printf("New Password:");
            scanf("%s", password);
            preferences = create_account(mm_users, key, password);
            if (preferences) {
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("\t\tWelcome %s!\t\t\n\n", key);
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                break;
            }
        }
    }
    while (1) {
        while (!preferences) {
            printf("Username:");
            scanf("%s", key);
            printf("Password:");
            scanf("%s", password);
            preferences = check_login_data(mm_users, key, password);
            if (preferences) {
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                printf("\t\tWelcome %s!\t\t\n\n", key);
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            }
        }
        scanf("%s", op);
        if (!strcmp(op, "EXIT")) {
            free(key);
            free(op);
            exit(0);
        } else if (!strcmp(op, "LOGOUT")) {
            preferences = NULL;
        } else if (!strcmp(op, "ADD")) {
            scanf("%s", isbn);
            add_preferences(ht_books, preferences, isbn);
        // } else if (!strcmp(op, "REMOVE")) {
        //     scanf("%s", &isbn);
        //     remove_preferences(preferences, isbn);
        } else if (!strcmp(op, "PRINT")) {
            print_preferences(preferences);
        // } else if (!strcmp(op, "SHOW")) {
        //     scanf("%d", &criteria);
        //     // show_books(ht/lists?, criteria);
        // } else if (!strcmp(op, "SORT")) {
        //     scanf("%d", &criteria);
        //     sort_preferences(preferences, criteria);
        } else if (!strcmp(op, "HELP")) {
            show_instructions();
        }
        // } else if (!strcmp(op, "DESCRIPTION")) {
        //     scanf("%s", isbn);
        //     // print_description(ht/lists, isbn);
        // } else if (!strcmp(op, "DESCRIBE")) {
        //     scanf("%s", isbn);
        //     // change_description(ht/lists, isbn);
        // } else if (!strcmp(op, "RATE")) {
        //     scanf("%s", isbn);
        //     // rate_book(ht/lists, isbn);
        // }
    }
    return 0;
}
