#include "books_management.h"

void print_book(book *desired_book)
{
    printf("Title: %s\n", desired_book->title);
    printf("Authors: %s\n", desired_book->authors);
    printf("ISBN: %s\n", desired_book->isbn);
    printf("Published Year: %d\n", desired_book->published_year);
    printf("Rating: %f\n", desired_book->rating);
    printf("Rating Count: %d\n", desired_book->rating_count);
    printf("Page Count: %d\n", desired_book->page_count);
    printf("Genre: %s\n", desired_book->genre);
    printf("Description: %s\n\n", desired_book->description);
}

void load_csv(hashtable_t *ht_books) {
    book new_book;
    char line[MAX_STRING_SIZE];
    FILE *input = fopen("/data/books.csv", "r");
    fgets(line, MAX_STRING_SIZE, input);
    while(fscanf(input, "%10[^~]~%[^~]~%[^~]~%[^~]~%d~%f~%d~%d~%[^\n]\n",
            new_book.isbn, new_book.title, new_book.authors, new_book.genre, 
            &new_book.published_year, &new_book.rating, &new_book.page_count, &new_book.rating_count, new_book.description) == 9)
        ht_put(ht_books, &new_book.isbn, sizeof(new_book.isbn), &new_book, sizeof(new_book));   
}

void print_isbn(hashtable_t *ht_books)
{
    char isbn[MAX_STRING_SIZE];
    linked_list_t *book_list;
    scanf("%s", isbn);
    // getchar();
    // printf("after getchar");
    // fgets(isbn, MAX_STRING_SIZE, stdin);
    // isbn[strlen(isbn) - 1] = '\0';
    // printf("[%s]", isbn);
    if (ht_has_key(ht_books, isbn)) {
        // printf("hewo");
		book_list = ht_get(ht_books, isbn);
        ll_node_t* temp = book_list->head;
        while(temp)
        {
            print_book(((book *)temp->data));
            temp = temp->next;
        }
    }
}