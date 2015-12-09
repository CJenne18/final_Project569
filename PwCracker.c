#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//#include <mpi.h>

#define CHAR_SET_SIZE 94

typedef enum { false, true } bool;
char password;
char *pw = NULL;
int counter=0;
bool crack = true;
bool found = false;

char getPassword(char * pass)
{
    
    printf("Enter password " );
    scanf("%s" , pass);
    return  * pass;
}

int checkPw(char * str)
{
    int a = 0;
    if(strcmp(&password, str) == 0)
    {
        a = 1;
    }
    
    return a ;
}

int crack_it(char *word)
{

    //trim \n & \r
    if( word[strlen(word)-1] == '\n' )
        word[strlen(word)-1] = '\0';
    
    if( word[strlen(word)-1] == '\r' )
        word[strlen(word)-1] = '\0';
    
    if (checkPw(word) != 0)
    {
        pw = word;
        return 0;
    }
    else
    {
        return 1;
    }
}


int dict_words(void)
{
    FILE *dict_words = fopen("/usr/share/dict/words", "r");
    char word[80];
    
    while (fgets ( word, sizeof(word), dict_words) != NULL)
    {
        int crack_it_return =  crack_it(word);
        counter++;
        
        if (crack_it_return == 0)
        {
            fclose(dict_words);
            return 0;
        }
    }
    
    fclose(dict_words);
    return 1;
}

int brute_force (void)
{
    int max_size = strlen(&password);
    
    int char_set[CHAR_SET_SIZE];
    
    for (int i = 0; i < CHAR_SET_SIZE; i++)
    {
        char_set[i] = (i + 33);
    }
    
    for (int this_size = 1; this_size <= max_size; this_size++)
    {
        if (!found)
        {
            char b_word[this_size + 1];
            
            for (int i = 0; i < this_size; i++)
            {
                b_word[i] = (char)char_set[0];
                
                if (i == (this_size - 1))
                {
                    b_word[i + 1] = '\0';
                }
            }
            
            int crack_out;
            crack_out = crack_seq(b_word, char_set, this_size, 0);
            
            if (crack_out == 0)
            {
                found = true;
            }
        }
    }
    
    return 0;
}

int crack_seq(char *guess, int *char_set, int this_size, int position)
{
    if (position == (this_size - 1)) 
    {
        for(int i = 0; i < (CHAR_SET_SIZE - 1); i++)
        {
            guess[position] = char_set[i];
            int crack_it_return =  crack_it(guess);
            counter++;
            
            if(crack_it_return == 0)
            {
                return 0;
            }
        }
        return 1;
    }
    else
    {
        int result;
        for(int i = 0; i < (CHAR_SET_SIZE - 1); i++)
        {
            guess[position] = (char)char_set[i];
            result = crack_seq(guess, char_set, this_size, (position + 1));
            if (result == 0)
            {
                return 0; //success
            }
            
            
        }
        return 1; //not found
    }
}

int main(void)
{
    printf("Enter password " );
    scanf("%s" , &password);
    
    time_t start, end;
    double dif;
    time (&start);
    
    //check dictionary
    if (!found)
    {
        if(crack)
        {
            printf("Checking a list of dictionary words...\n");
        }
        int dict_words_return = dict_words();
        if (dict_words_return == 0)
        {
            found = true;
        }
    }
    
    //call brute_force
    if (!found)
    {
        if(crack)
        {
            printf("Starting a brute force attack...\n");
        }
        int brute_force_return = brute_force();
        if (brute_force_return == 0)
        {
            found = true;
        }
    }
    
    if (found)
    {
        if (crack)
        {
            printf("Found it The password is: %s\n", pw);
        }
        else
        {
            printf("%s\n", pw);
        }
    }
    else
    {
        if (crack)
        {
            printf("A match could not be found :(\n");
        }
    }
    
    //end timer
    time (&end);
    dif = difftime(end, start);
    
    
    if (crack)
    {
        printf("There were %d combinations checked.\n", counter);
        printf("The program ran for: %.0lf seconds.\n", dif);
        printf("Processed: %.0f combinations per second.\n", (counter/dif));
    }
    return 0;
}