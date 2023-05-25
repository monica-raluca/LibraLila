#include "books_management.h"

void print_book(book *desired_book)
{
    printf("Title: %s\n", desired_book->title);
    printf("Authors: %s\n", desired_book->authors);
    printf("ISBN: %s\n", desired_book->isbn);
    printf("Published Year: %d\n", desired_book->published_year);
    printf("Rating: %f\n", desired_book->rating);
    printf("Page Count: %d\n", desired_book->page_count);
    printf("Genre: %s\n", desired_book->genre);
    printf("Description: %s\n\n", desired_book->description);
}