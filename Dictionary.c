#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char word[32];
    int mean_num;
    char *word_mean[10];
} Entry;

typedef struct {
    int entry_num;
    Entry *ptr[10000];
} Dictionary;

Dictionary createDictionary()
{
    //creation of a dictionary containing 0 entries
    Dictionary somedict;
    somedict.entry_num = 0;
    return somedict;
}

Entry* searchWord (char word[], Dictionary *dict)
 {
    //searching for a provided word using a sequential search
    //return pointer to the entry corresponding to the word if it is found in the dictionary
    //if not, return NULL
    int i;
    for (i = 0; i < dict->entry_num; i++) {
        if(strcmp(dict->ptr[i]->word, word) == 0)
            return dict->ptr[i];
    }
    return NULL;
}

int addDefinition (char word[], char meaning[], Dictionary *dict)
{
    int success = 0;
    /*
     * a) if the word is not found in the dictionary AND if the dictionary is not full,
     *    1) add the provided word to the dictionary
     *    2) increase the number of entries by 1
     *    3) set the number of meanings to 0
     *    4) count the words added
     */
    if (searchWord(word, dict) == NULL && dict->entry_num < 10000) {
        Entry *new_entry = (Entry*)malloc(sizeof(Entry));
        strcpy(new_entry->word, word);
        new_entry->mean_num = 0;
        dict->ptr[dict->entry_num] = new_entry;
        dict->entry_num++;
        success++;
    }
    /*
     * b) if the number of meanings is less than 10 for found or added word
     *    1) add the provided meaning to the dictionary
     *    2) increase the number of meanings by 1
     *    3) count the meanings added
     */
    Entry *en = searchWord(word, dict);
    if (en != NULL && en->mean_num < 10) {
        char *new_mean = (char*)malloc(sizeof(char));
        new_mean = meaning;
        en->word_mean[en->mean_num] = new_mean;
        en->mean_num++;
        success++;
    }
    /*
     * if no entry and no meaning is added, return 0
     * if no entry but 1 meaning is added, return 1
     * if new entry is added and new definition is added, return 2
     */
    if (success == 0)
        return 0;
    else if (success == 1)
        return 1;
    else
        return 2;
}

int loadDefinitions(char fileName[32], Dictionary *dict)
{
    int i, j, y, k, scs, success = 0;
    
    //opening the provided file for reading
    FILE *file;
    file = fopen(fileName, "r");
    
    if (file == NULL) {
        //if file is not found, print the message and return -1
        printf("Problem opening file.");
        return 1;
    }
    char ch;
    
    k = 0;
    while(1) {
        i = 0;
        j = 0;
        y = 0;
        char whole[1001], nword[1001][1001], ndef[1001][1001];
        int exit = 0;
        //getting the whole line including '.'
        do {
            ch = getc(file);
            if (ch == -1) {
                exit = 1;
                break;
            }
            if (ch == '\n')
                continue;
            whole[i++] = ch;
        }while(ch != '.');
        
        //if the getc function encounters the end of the file, break from the loop
        if (exit == 1)
            break;
        whole[i] = '\0';
        
        //assigning characters to the word up to space
        do {
            nword[k][j] = whole[j];
            j++;
        } while(whole[j] != ' ');
        nword[k][j] = '\0';
        
        //skipping spaces between the word and its definition
        do {
            j++;
        } while (whole[j] == ' ');
        
        //assigning characters to the definition up to the end of the line
        do {
            ndef[k][y] = whole[j];
            j++;
            y++;
        } while (whole[j] != '\0');
        ndef[k][y] = '\0';
        
        //getting the number of meanings loaded
        scs = addDefinition(nword[k], ndef[k], dict);
        if (scs > 0)
            success++;
        k++;
    }
    fclose(file);
    //returns the total number of meaning definitions added to the dictionary
    return success;
}

void sortDictionary(Dictionary *dict)
{
    int i = 0;
    //sorting entries in the dictionary using selection sort
    for (i = 0; i < dict->entry_num; i++) {
        int j, frt = i;
        for (j = i + 1; j < dict->entry_num; j++) {
            if(strcmp(dict->ptr[j]->word, dict->ptr[frt]->word) < 0)
                frt = j;
        }
        //swapping the pointers to entries
        Entry *save = dict->ptr[i];
        dict->ptr[i] = dict->ptr[frt];
        dict->ptr[frt] = save;
    }
}

int main()
{
    
    int count;
    Entry *entryPtr;
    int i, j;
    
    // TESTING TASK 1 - createDictionary
    Dictionary dict = createDictionary();
    printf("\nTASK 1 - createDictionary.");
    printf("\nDictionary created.");
    
    // TESTING TASK 2 - searchWord in an empty Dictionary
    printf("\n\nTASK 2 - searchWord in an empty Dictionary.");
    entryPtr = searchWord ("include", &dict);
    if (entryPtr != NULL) {
        printf("Word found: %s", entryPtr->word);
        for(i = 1; i <= entryPtr->mean_num; i++) {
            printf("№%i Meaning of the word: %s", i, entryPtr->word_mean[i-i]);
        }
    } else {
        printf("\nWord 'include' is not found in the dictionary.");
    }
    
    // TESTING TASK 3 - addDefinition - new Entries
    printf("\n\nTASK 3 - addDefinition - new Entries.");
    count = addDefinition ("include", "(verb) def1", &dict);
    count = addDefinition ("assist", "(verb) def2", &dict);
    count = addDefinition ("house", "(noun) def3", &dict);
    count = addDefinition ("camera", "(noun) def4", &dict);
    count = addDefinition ("valid", "(adjective) def5", &dict);
    
    printf("\n\nThe the value returned by the last addition: is %i.", count);
    
    //printing words and their definitions pointed by dict
    printf("\n");
    for (i = 1; i <= dict.entry_num; i++) {
        printf("\t %i. %s\n", i, dict.ptr[i-1]->word);
        for (j = 1; j <= dict.ptr[i-1]->mean_num; j++)
            printf("\t\t %i.%i %s\n", i, j, dict.ptr[i-1]->word_mean[j-1]);
        entryPtr++;
    }
    
    // TESTING TASK 2 - searchWord in a non-empty Dictionary
    printf("\n \nTASK 2 - searchWord in a non-empty Dictionary.");
    entryPtr = searchWord ("exam", &dict);
    if (entryPtr != NULL) {
        printf("Word found: %s", entryPtr->word);
        for (i = 1; i <= entryPtr->mean_num; i++)
            printf("№%i Meaning of the word: %s", i, entryPtr->word_mean[i-i]);
    } else {
        printf("\nWord 'exam' is not found in the dictionary.");
    }
    entryPtr = searchWord ("include", &dict);
    if (entryPtr != NULL) {
        printf("\nWord found: %s", entryPtr->word);
        for (i = 1; i <= entryPtr->mean_num; i++)
            printf("\n№%i Meaning of the word: %s", i, entryPtr->word_mean[i-1]);
    } else {
        printf("\nWord 'include' is not found in the dictionary.");
    }
    
    // TESTING TASK 3 - addDefinition - new Meaning Definitions for already stored entries
    printf("\n\nTASK 3 - addDefinition - new Meaning Definitions for already stored entries.");
    count = addDefinition ("house", "(adjective) def6", &dict);
    count = addDefinition ("house", "(adjective) def7", &dict);
    count = addDefinition ("house", "(adjective) def8", &dict);
    
    printf("\n\nThe the value returned by the last addition: is %i.", count);
    
    printf("\n\nPrint the entries in the Dictionary:");
    
    //printing words and their definitions pointed by dict
    printf("\n");
    for (i = 1; i <= dict.entry_num; i++) {
        printf("\t %i. %s\n", i, dict.ptr[i-1]->word);
        for (j = 1; j <= dict.ptr[i-1]->mean_num; j++)
            printf("\t\t %i.%i %s\n", i, j, dict.ptr[i-1]->word_mean[j-1]);
        entryPtr++;
    }
    
    printf("\nSome entries have multiple definitions.");
    
    // TESTING TASK 3 - addDefinition - full Meaning Definitions (10): last definition not added
    printf("\n\nTASK 3 - addDefinition - full Meaning Definitions (10): last definition not added.");
    count = addDefinition ("house", "(adjective) def9", &dict);
    count = addDefinition ("house", "(adjective) def10", &dict);
    count = addDefinition ("house", "(adjective) def11", &dict);
    count = addDefinition ("house", "(adjective) def12", &dict);
    count = addDefinition ("house", "(adjective) def13", &dict);
    count = addDefinition ("house", "(adjective) def14", &dict);
    count = addDefinition ("house", "(adjective) def15", &dict);
    
    printf("\n\nThe the value returned by the last addition: is %i.", count);
    
    printf("\n\nPrint the entries in the Dictionary:");
    
    //printing words and their definitions pointed by dict
    printf("\n");
    for (i = 1; i <= dict.entry_num; i++) {
        printf("\t %i. %s\n", i, dict.ptr[i-1]->word);
        for (j = 1; j <= dict.ptr[i-1]->mean_num; j++)
            printf("\t\t %i.%i %s\n", i, j, dict.ptr[i-1]->word_mean[j-1]);
        entryPtr++;
    }
    
    printf("\nSome entries have multiple definitions.");
    
    // TESTING TASK 4 - loadDefinition - full Meaning Definitions for 'house', only 'language' definitions added
    printf("\n\nTASK 4 - addDefinition - full Meaning Definitions for 'house', only 'language' definitions added.");
    int loaded = loadDefinitions("entryList.txt", &dict);
    
    if (loaded > 0)
        printf("\nLoaded %i entries.", loaded);
    else
        printf("\nNothing has been loaded.");
    
    //printing words and their definitions pointed by dict
    printf("\n");
    for (i = 1; i <= dict.entry_num; i++) {
        printf("\t %i. %s\n", i, dict.ptr[i-1]->word);
        for (j = 1; j <= dict.ptr[i-1]->mean_num; j++)
            printf("\t\t %i.%i %s\n", i, j, dict.ptr[i-1]->word_mean[j-1]);
        entryPtr++;
    }
    
    // TESTING TASK 5 - sortDictionary
    printf("\n\nTASK 5 - sortDictionary.");
    sortDictionary(&dict);
    
    //printing words and their definitions pointed by dict
    printf("\n");
    for (i = 1; i <= dict.entry_num; i++) {
        printf("\t %i. %s\n", i, dict.ptr[i-1]->word);
        for (j = 1; j <= dict.ptr[i-1]->mean_num; j++)
            printf("\t\t %i.%i %s\n", i, j, dict.ptr[i-1]->word_mean[j-1]);
        entryPtr++;
    }
    
    printf("\n\nProgram terminated after sorting the Dictionary.");
    return 0;
}
