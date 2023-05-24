#include "login.h"

// adaugam totul intr-un fisier si facem afisarea direct din fisier
show_instructions()
{
    printf("EXIT - Exit the program and free memory\n");
    printf("ADD [isbn] - Add a book on your list of preferences based on isbn");
    printf("REMOVE [isbn] - Remove a book from your list of preferences based
           on its isbn\n");
    printf("PRINT - Print the current list of preferences\n");
    printf("SHOW [criteria_no] [criteria] - Show all the books in the library
           based on the following criterias:\n");
    printf("0 - Author\n1 - Title\n2 - Genre\n3 - Release date between\n4 - Rating between\n");
    printf("SORT [criteria_no] - Sort the books in the list of preferences by the
           following criterias:\n");
    printf("0 - Author\n1 - Title\n2 - Genre (Alphabetically)\n3 - Release date\n4 - Ratings\n");
    printf("DESCRIPTION [isbn] - View the description of a book with a particular isbn")
    printf("DESCRIBE [isbn] - Add a different description to the book with a specific isbn\n");
    printf("RATE [isbn] - Add a rating to a book with the given isbn\n");
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
