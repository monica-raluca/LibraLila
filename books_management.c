#include "books_management.h"

void print_book(book *desired_book)
{
    printf("ISBN: %s\n", desired_book->isbn);
    printf("Title: %s\n", desired_book->title);
    printf("Authors: %s\n", desired_book->authors);
    printf("Published Year: %d\n", desired_book->published_year);
    printf("Rating: %.2f\n", desired_book->rating);
    printf("Rating Count: %d\n", desired_book->rating_count);
    printf("Page Count: %d\n", desired_book->page_count);
    printf("Genre: %s\n", desired_book->genre);
    printf("Description: %s\n\n", desired_book->description);
}

void load_csv(hashtable_t *ht_books) {
    book new_book;
    char line[MAX_STRING_SIZE];
    FILE *input = fopen("data/books.csv", "r");
    fgets(line, MAX_STRING_SIZE, input);
    while(fscanf(input, "%10[^~]~%[^~]~%[^~]~%[^~]~%d~%f~%d~%d~%[^\n]\n",
            new_book.isbn, new_book.title, new_book.authors, new_book.genre, 
            &new_book.published_year, &new_book.rating, &new_book.page_count, &new_book.rating_count, new_book.description) == 9) {
        ht_put(ht_books, &new_book.isbn, sizeof(new_book.isbn), &new_book, sizeof(new_book));   
    }
    fclose(input);
}

void print_isbn(hashtable_t *ht_books, char *isbn)
{
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        ll_node_t* temp = ((linked_list_t *)book_info->value)->head;
        print_book((book *)temp->data);
    }
}

void print_description(hashtable_t *ht_books, char *isbn)
{
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        ll_node_t* temp = ((linked_list_t *)book_info->value)->head;
        printf("%s\n", ((book *)temp->data)->description);
    }
}

void change_description(hashtable_t *ht_books, char *isbn)
{
    char new_description[MAX_STRING_SIZE];
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        ll_node_t* temp = ((linked_list_t *)book_info->value)->head;
        fgets(new_description, MAX_STRING_SIZE, stdin);
        memcpy(((book *)temp->data)->description, new_description, MAX_STRING_SIZE);
    }
}

void rate_book(hashtable_t *ht_books, char *isbn)
{
    float user_rating;
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        ll_node_t* temp = ((linked_list_t *)book_info->value)->head;
        scanf("%f", &user_rating);
        ((book *)temp->data)->rating = (((book *)temp->data)->rating * ((book *)temp->data)->rating_count + user_rating) / (((book *)temp->data)->rating_count + 1);
        ((book *)temp->data)->rating_count++;
    }
}
