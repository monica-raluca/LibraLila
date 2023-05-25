#include "login.h"

void load_login_data(hashtable_t *mm_users)
{
    char user[100];
    unsigned int pass;
    FILE *input = fopen("/data/user_database.csv", "r");
    while(fscanf(input, "%[^~]~%u", user, &pass) == 2) {
        printf("%s %u\n", user, pass);
        mm_put(mm_users, user, sizeof(user), &pass, sizeof(pass));
    }    
}
