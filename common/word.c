/**
 * @file word.c
 * @author Rehoboth Okorie (www.rehoboth.link)
 * @brief implements items defined in word.h (module providing the method normalizeWord which converts a word to lowercase)
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <ctype.h>
#include <string.h>

/* function to normalize a word (convert the word to lower case) */
/* see word.h for more information */
int normalizeWord(char *word) {
    if (word == NULL) { // validate arguments
        return -1;
    }
    int limit = strlen(word);   // get word lenght
    for (int i = 0; i < limit; i++) {   // loop till word length
        if (isalpha(word[i])) { // only make lowercase if char is alphabet
            word[i] = (char) tolower(word[i]);
        }
    }
    return 0;
}
