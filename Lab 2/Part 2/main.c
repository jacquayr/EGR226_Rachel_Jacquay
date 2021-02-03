/**************************************************************************************
* Author:       Rachel Jacquay
* Course:       EGR 226-902
* Date:         01/27/2021
* Project:      Lab 2 Part 2
* File:         main_part2.c
* Description:  This program takes in a file of type .csv, and also user input to
                create a search engine of books, using their title, author, ISBN,
                page number, and year published. It loops until the user tells it to
                stop by setting the variable 'loop' equal to 0. Error checking was
                used for all inputs by the user as well as the file.
**************************************************************************************/

#include <stdio.h>      // preprocessor directives
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX 500         // macro

typedef struct {                // creating struct
        char title[225];
        char author_name[50];
        char ISBN[10];
        int pages;
        int year_published;
    } book;

int parse_file(char filename[], book book_array[]);             // function prototypes
void print_book(book my_book);
void search_title(book book_title[], int n, char title[]);
void search_author(book book_author[], int n, char author_name[]);
void search_ISBN(book book_ISBN[], int n, char ISBN[]);

int main() {                // main function
    book my_book;           // declaring variables
    book book_array[360];
    char filename[MAX];
    char userin[255];
    int b, num;
    int status = 1;
    int loop = 1;

    strcpy(filename, "BookList.csv");       // set filename to BookList.csv

    b = parse_file(filename, book_array);       // set i equal to the number of books read in

    do {                                                            // do all of this while the user still wants to loop
        printf("Which process would you like to search by?\n");     // prompt user
        printf("Please enter one of the following numbers:\n");
        printf("[0] Search by Title\n");
        printf("[1] Search by Author\n");
        printf("[2] Search by ISBN\n");
        printf("[3] Exit code\n\n");

        do {                                // do all of this while the scanf and numbers scanned in are valid
            fflush(stdin);
            status = scanf("%d", &num);     // checking for scanf to be valid

            if (status == 0 || num < 0 || num > 3) {        // if invalid, redo
                printf("Incorrect value\n");
                printf("Please enter a '0' or '1' or '2' or '3'\n");
                fflush(stdin);
            }
        } while (status == 0 || num < 0 || num > 3);

        if (num == 0) {                     // search by title
            printf("\nUser entered 0\n");

                fflush(stdin);
                printf("\nPlease enter a case-sensitive title\n\n");
                scanf("%[^\n]s", userin);                        // get the string
                search_title(book_array, b, userin);        // call search by title function
        }

        else if (num == 1) {                // search by author
            printf("\nUser entered 1\n");

                fflush(stdin);
                printf("\nPlease enter a case-sensitive author\n\n");
                scanf("%[^\n]s", userin);                        // get the string
                search_author(book_array, b, userin);       // call search by author function
        }

        else if (num == 2) {                // search by ISBN
            printf("\nUser entered 2\n");

                fflush(stdin);
                printf("\nPlease enter an ISBN\n\n");
                scanf("%[^\n]s", userin);                        // get the string
                search_ISBN(book_array, b, userin);         // call search by ISBN function
        }

        else if (num == 3) {                // end the program entirely
            printf("\nUser entered 3\n");
            printf("Goodbye!");
            loop = 0;
        }

        printf("\n");

    } while (loop != 0);    // loop until user wants to end program

    return 0;
}

/*  parse_file
    Description: This function opens, stores, and closes the file, while
    also parsing its contents into words and using those for the search
    engine. It returns the total number of books found.
    Inputs: filename[], book_array[]
    Outputs: i
*/
int parse_file(char filename[], book book_array[]) {
    char buffer[512];                                       // Create temporary string buffer variable
    int i = 0;
    char* ptr;
    FILE *infile;                                           // Attempt to open file

    infile = fopen(filename, "r");

    if (infile == NULL)                                     // Return 0 (failure) if file could not open
        return -1;

    while (fgets(buffer, 512, infile)) {                    // Loop collecting each line from the file
        ptr = strtok(buffer, ",");                          // Parse string by commas and newline
            strcpy(book_array[i].title, ptr);               // First parse is title

        ptr = strtok(NULL, ",\n");                          // repeat for author name
            strcpy(book_array[i].author_name, ptr);

        ptr = strtok(NULL, ",\n");                          // repeat for ISBN
            strcpy(book_array[i].ISBN, ptr);

        ptr = strtok(NULL, ",\n");                          // repeat for pages
        if (strcmp(ptr, "N/A"))                             // if N/A, set ptr to an int and store the value in ptr
            book_array[i].pages = atoi(ptr);
        else if (strcmp(ptr, "N/A") == 0)                   // if it is 0 to start, output 0
            book_array[i].pages = 0;

        ptr = strtok(NULL,",\n");                          // repeat for year published
        if (strcmp(ptr,"N/A"))                             // same as pages
            book_array[i].year_published = atoi(ptr);
        else if (strcmp(ptr, "N/A") == 0)
            book_array[i].year_published = 0;

        i++;        // increment i to see how many books are found
    }

    fclose(infile);     // close file

    return i;       // return how many books are found total
}

/*  print_book
    Description: This function prints out the book info whenever it is called
    in any of other functions.
    Inputs: my_book
    Outputs: none
*/
void print_book(book my_book) {                         // printing book function
    printf("\nTitle:       %s\n", my_book.title);       // print title
    printf("Author:      %s\n", my_book.author_name);   // print author name
    printf("ISBN:        %s\n", my_book.ISBN);          // print ISBN

    if (my_book.pages == 0)                             // if struct is 0
        printf("Pages:       N/A\n");                   // print N/A

    else if (my_book.pages != 0)                        // if struct is not 0
        printf("Pages:       %d\n", my_book.pages);     // print value

    if (my_book.year_published == 0)                    //if struct is 0
        printf("Year Published:       N/A\n");          // print N/A

    else if (my_book.year_published != 0)                               // if struct is not 0
        printf("Year Published:       %d\n", my_book.year_published);   // print value
}

/*  search_title
    Description: This function determines if the title given by the user
    matches any title in the .csv file.
    Inputs: book_title, n, title
    Outputs: none
*/
void search_title(book book_title[], int n, char title[]) {     // search title function definition
    int i;
    int var;
    char outcome;

    for (i = 0; i <= n; i++) {                              // until index equals n number of books
        outcome = (strstr(book_title[i].title, title));     // set outcome equal to strstr of book_title[i] and title

        if (outcome) {                                      // if outcome == 1
            print_book(book_title[i]);                      // print title
            var++;                                          // var tells user how many books were found
            }
        }

    if (var == 0) {                         // if var == 0
        printf("\nNo results found\n");     // no books found
    }
}

/*  search_author
    Description: This function determines if the author name given by the user
    matches any author names in the .csv file.
    Inputs: book_author, n, author_name
    Outputs: none
*/
void search_author(book book_author[], int n, char author_name[]) {     // search author function definitions
    int i;
    int var;
    char outcome;

    for (i = 0; i <= n; i++) {                                          // until index is equal to n number of books
        outcome = (strstr(book_author[i].author_name, author_name));    // set outcome equal to strstr of book_author[i] and author

        if (outcome) {                      // if outcome == 1
            print_book(book_author[i]);     // print author
            var++;                          // var tells user how many books were found
        }
    }

    if (var == 0) {                         // if var == 0
        printf("\nNo results found\n");     // no books found
    }
}

/*  search_ISBN
    Description: This function determines if the ISBN given by the user
    matches any ISBN in the .csv file.
    Inputs: book_ISBN, n, ISBN
    Outputs: none
*/
void search_ISBN(book book_ISBN[], int n, char ISBN[]) {        // search ISBN function definition
    int i;
    int var;
    char outcome;

    for (i = 0; i <= n; i++) {                          // until index is equal to n number of books
        outcome = (strstr(book_ISBN[i].ISBN, ISBN));    // set outcome equal to strstr of book_ISBN[i] and ISBN

        if (outcome) {                  // if outcome == 1
            print_book(book_ISBN[i]);   // print ISBN
            var++;                      // var tells user how many books were found
        }
    }

    if (var == 0) {                         // if var == 0
        printf("\nNo results found\n");     // no books found
    }
}
