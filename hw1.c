/**
 * This program is a rudimentary cache that stores the
 * words of a given text file as a hash-table
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 128
#define MAX_LINE_LEN 1048


/**
 * Takes a word and the size of the cache to determine a hash calue.
 * @param word: word being hashed.
 * @param size: size of cache.
 * @return: integer hash value of word for given cache size.
 */
 int hash_code(char* word, int size)
 {
     int sum = 0;
     int i = 0;

     while(*(word + i) != '\0')
     {
         sum += *(word + i);
         i++;
     }
     
     return sum % size;
 }

/**
 * Takes a word that is read and strips all non-letter characters out.
 * @param word: word being stripped.
 */
 void strip_word(char *word)
 {
     int pos = 0;
     int i = 0;
     while(*(word + i) != '\0')
     {
         if(isalpha(*(word + i)))
         {
             *(word + pos) = *(word + i);
             pos++;
         }
         i++;
     }
     *(word + pos) = '\0';
 }
 
 /**
  * Takes the next word from the current line, and moves the starting
  * position of the line over.
  */
void next_word(char *line, char *word)
{
    int pos = 0;
    while(isalnum(*(line + pos)))
    {
        *(word + pos) = *(line + pos);
        pos++;
    }
    *(word + pos) = '\0';
    memmove(line, line + pos + 1, strlen(line) - pos + 1);
}


/**
 * Reads file and caches results.
 * @param filename: name of file to open.
 * @param cache: cache words are being saved to.
 * @param size: size of cache.
 */
void read_file(char *filename, char **cache, int size)
{
    FILE* read = fopen(filename, "r");

    if (read == NULL)
    {
        fprintf(stderr, "ERROR: input file <%s> does not exist\n", filename);
        exit(1);
    }

    char* line = calloc(MAX_LINE_LEN, sizeof(char));

    while(fgets(line, MAX_LINE_LEN, read))
    {
        while(strlen(line) >= 3)
        {
            char* word = calloc(MAX_WORD_LEN, sizeof(char));
            next_word(line, word);

            if(strlen(word) >= 3)
            {
                int hash = hash_code(word, size);
                if(*(cache + hash))
                {
                    *(cache + hash) = realloc(*(cache + hash), MAX_WORD_LEN * sizeof(char));
                    printf("Word \"%s\" ==> %d (realloc)\n", word, hash);
                }
                else
                {
                    *(cache + hash) = calloc(MAX_WORD_LEN, sizeof(char));
                    printf("Word \"%s\" ==> %d (calloc)\n", word, hash);
                }
                strcpy(*(cache + hash), word);
            }
            free(word);
        }
    }

    free(line);

    for(int i = 0; i < size; i++)
    {
        if(*(cache + i))
        {
            printf("Cache index %d ==> \"%s\"\n", i, *(cache + i));
            free(*(cache + i));
        }
    }
}

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    // Ensure input format is correct
    if(argc != 3)
    {
        fprintf(stderr, "ERROR: Invalid arguments\n"\
        "USAGE: ./a.out <size> <filename>\n");
    }

    int size = atoi(*(argv + 1));
    // Stop the program if an invalid size is defined
    if(size <= 0)
    { 
        fprintf(stderr, "Invalid Size: %s\n", *(argv + 1));
        return -1;
    }

    char **cache = calloc(size, sizeof(char*));

    read_file(*(argv + 2), cache, size);

    free(cache);
}
