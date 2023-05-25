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
        book* temp = book_info->value;
        print_book(temp);
    }
}

void print_description(hashtable_t *ht_books, char *isbn)
{
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        book* temp = book_info->value;
        printf("%s\n", temp->description);
    }
}

void change_description(hashtable_t *ht_books, char *isbn)
{
    char new_description[MAX_STRING_SIZE];
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        book* temp = book_info->value;
        fgets(new_description, MAX_STRING_SIZE, stdin);
        memcpy(temp->description, new_description, MAX_STRING_SIZE);
    }
}

void rate_book(hashtable_t *ht_books, char *isbn)
{
    float user_rating;
    info *book_info;
    if (ht_has_key(ht_books, isbn)) {
		book_info = ht_get(ht_books, isbn);
        book* temp = book_info->value;
        scanf("%f", &user_rating);
        temp->rating = (temp->rating * temp->rating_count + user_rating) / (temp->rating_count + 1);
        temp->rating_count++;
    }
}

void show_author(hashtable_t *ht_books)
{
    char author[MAX_STRING_SIZE];
    printf("hello\n");
    fgets(author, MAX_STRING_SIZE, stdin);
    for (unsigned int i = 0; i < ht_books->hmax; i++) {
        linked_list_t *bucket = ht_books->buckets[i];
        ll_node_t *current = bucket->head;
        while (current) {
            book *current_book = (book *)current->data;
            if (strcmp(current_book->authors, author) == 0) {
                print_book(current_book);
            }
            current = current->next;
        }
    }
}

void show_books(hashtable_t *ht_books, int criteria)
{
    if (criteria == 0) {
        show_author(ht_books);
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
