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

void show_authors(hashtable_t *ht_books, char *authors)
{
    for (unsigned int i = 0; i < ht_books->hmax; i++) {
        linked_list_t *list = ht_books->buckets[i];
        ll_node_t *cursor = list->head;
        for (unsigned int j = 0; j < list->size; j++) {
            book *book_info = (book *)(((info *)cursor->data)->value);
            if (!strcmp(book_info->authors, authors))
                print_book(book_info);
            cursor = cursor->next;
        }
    }
}

void show_title(hashtable_t *ht_books, char *title)
{
    for (unsigned int i = 0; i < ht_books->hmax; i++) {
        linked_list_t *list = ht_books->buckets[i];
        ll_node_t *cursor = list->head;
        for (unsigned int j = 0; j < list->size; j++) {
            book *book_info = (book *)(((info *)cursor->data)->value);
            if (!strcmp(book_info->title, title))
                print_book(book_info);
            cursor = cursor->next;
        }
    }
}

void show_genre(hashtable_t *ht_books, char *genre)
{
    for (unsigned int i = 0; i < ht_books->hmax; i++) {
        linked_list_t *list = ht_books->buckets[i];
        ll_node_t *cursor = list->head;
        for (unsigned int j = 0; j < list->size; j++) {
            book *book_info = (book *)(((info *)cursor->data)->value);
            if (!strcmp(book_info->genre, genre))
                print_book(book_info);
            cursor = cursor->next;
        }
    }
}

void show_date(hashtable_t *ht_books, int start, int end)
{
    for (unsigned int i = 0; i < ht_books->hmax; i++) {
        linked_list_t *list = ht_books->buckets[i];
        ll_node_t *cursor = list->head;
        for (unsigned int j = 0; j < list->size; j++) {
            book *book_info = (book *)(((info *)cursor->data)->value);
            if (book_info->published_year >= start && book_info->published_year <= end)
                print_book(book_info);
            cursor = cursor->next;
        }
    }
}

void show_rating(hashtable_t *ht_books, float start, float end)
{
    for (unsigned int i = 0; i < ht_books->hmax; i++) {
        linked_list_t *list = ht_books->buckets[i];
        ll_node_t *cursor = list->head;
        for (unsigned int j = 0; j < list->size; j++) {
            book *book_info = (book *)(((info *)cursor->data)->value);
            if (book_info->rating >= start && book_info->rating <= end)
                print_book(book_info);
            cursor = cursor->next;
        }
    }
}

void show_books(hashtable_t *ht_books)
{
    int criteria;
    scanf("%d ", &criteria);
    if (criteria == 0) {
        char *authors = calloc(MAX_STRING_SIZE, sizeof(char));
        fgets(authors, MAX_STRING_SIZE, stdin);
        authors[strlen(authors) - 1] = '\0';
        show_authors(ht_books, authors);
        free(authors);
    } else if (criteria == 1) {
        char *title = calloc(MAX_STRING_SIZE, sizeof(char));
        fgets(title, MAX_STRING_SIZE, stdin);
        title[strlen(title) - 1] = '\0';
        show_title(ht_books, title);
        free(title);
    } else if (criteria == 2) {
        char *genre = calloc(MAX_STRING_SIZE, sizeof(char));
        fgets(genre, MAX_STRING_SIZE, stdin);
        genre[strlen(genre) - 1] = '\0';
        printf("%s\n", genre);
        show_genre(ht_books, genre);
        free(genre);
    } else if (criteria == 3) {
        int start, end;
        scanf("%d %d", &start, &end);
        show_date(ht_books, start, end);
    } else if (criteria == 4) {
        float start, end;
        scanf("%f %f", &start, &end);
        show_rating(ht_books, start, end);
    } else {
        printf("Invalid criteria\n");
        return;
    }
}
