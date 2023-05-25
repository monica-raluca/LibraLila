#include "books_management.h"

// adaugam totul intr-un fisier si facem afisarea direct din fisier
void show_instructions()
{
    FILE *fhelp = fopen("/data/help.txt", "r");
    char *buffer[MAX_STRING_SIZE];
    for (int i = 1; i <=19; i++) {
        fgets(buffer, MAX_STRING_SIZE, fhelp);
        puts(buffer);
    }
}

int main(void)
{
    char *key, *op, *password, isbn[11];
    int criteria;
    password = calloc(1, MAX_STRING_SIZE);
    key = calloc(1, MAX_STRING_SIZE);
    op = calloc(1, MAX_STRING_SIZE);
    DIE(!key || !op || !password, "Unable to allocate memory\n");
    // load la datele de login
    // load la carti
    // blocat in login page pana la introducere unui login valid
    linked_list_t *preferences = NULL;
    printf("Do you have an account? [Y/N]\n");
    while (1) {
        scanf("%s", isbn);
        if (isbn == 'Y')
            break;
        else if (isbn == 'N') {
            printf("New Username:");
            scanf("%s", key);
            printf("New Password:");
            scanf("%s", password);
            preferences = create_account(key, password);
        }
    }
    while (1) {
        while (!preferences) {
            printf("Username:");
            scanf("%s", key);
            printf("Password:");
            scanf("%s", password);
            preferences = check_login_data(key, password);
        }
        scanf("%s", op);
        if (!strcmp(op, "EXIT")) {
            free(key);
            free(op);
            exit(0);
        } else if (!strcmp(op, "LOGOUT")) {
            preferences = NULL;
        } else if (!strcmp(op, "ADD")) {
            scanf("%s", &isbn);
            add_preferences(preferences, isbn);
        } else if (!strcmp(op, "REMOVE")) {
            scanf("%s", &isbn);
            remove_preferences(preferences, isbn);
        } else if (!strcmp(op, "PRINT")) {
            print_preferences(preferences);
        } else if (!strcmp(op, "SHOW")) {
            scanf("%d", &criteria);
            // show_books(ht/lists?, criteria);
        } else if (!strcmp(op, "SORT")) {
            scanf("%d", &criteria);
            sort_preferences(preferences, criteria);
        } else if (!strcmp(op, "HELP")) {
            show_instructions();
        } else if (!strcmp(op, "DESCRIPTION")) {
            scanf("%s", isbn);
            // print_description(ht/lists, isbn);
        } else if (!strcmp(op, "DESCRIBE")) {
            scanf("%s", isbn);
            // change_description(ht/lists, isbn);
        } else if (!strcmp(op, "RATE")) {
            scanf("%s", isbn);
            // rate_book(ht/lists, isbn);
        }
    }
    return 0;
}
