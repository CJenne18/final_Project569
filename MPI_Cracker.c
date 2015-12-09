#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define CHAR_SET_SIZE 94

typedef enum { false, true } bool;
char password;
char *pw ;
char wwp[100];
char * tick;
int counter=0;
bool crack = true;
bool found = false;

void getPassword(char pass);
int checkPw(char * str);
int crack_it(char *word);
int dict_words(void);
int crack_seq(char *guess, int *char_set, int size, int position);
int brute_force (void);
void concatenate(char p[], char q[]);


int main(void)
{
    time_t start, end;
    double dif;
    int MAX_STRING = 100;
    char twD[MAX_STRING];
    char twD2[MAX_STRING];
    char twD3[MAX_STRING];
    char       greeting[MAX_STRING];  /* String storing message */
    int        comm_sz;               /* Number of processes    */
    int        my_rank;               /* My process rank        */
    int q;
    
    printf("Enter password " );
    scanf("%s" , &password);
    
    int i, count, j;
    int dest = 0;
    count = 0;
    
    tick = &password;
    
    /* Start up MPI */
    MPI_Init(NULL, NULL);
    
    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    
    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    time (&start);


    if(my_rank == 0)
    {
        for(dest = 1 ; dest < comm_sz; dest++)
        {
            for(i = 0; i < strlen(tick)/comm_sz; i++)
            {
                char subbuff[3];
                memcpy(subbuff,&tick[count],count+2);
                subbuff[2] = '\0';
                sprintf(wwp ,subbuff);
                MPI_Send(wwp, strlen(&password), MPI_CHAR, dest, 0 ,MPI_COMM_WORLD);
                count = count + 2;
            }
        }
    }

    if(my_rank != 0)
    {
        for(j = 0; j < strlen(&password)/comm_sz; j++)
        {
       
            MPI_Recv(twD, MAX_STRING, MPI_CHAR, 0 , 0 ,MPI_COMM_WORLD,MPI_STATUS_IGNORE );
            concatenate(twD2, twD);
        }
    }

    if(my_rank ==0)
    {
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
                printf("Found it! The password is: %s\n", tick);
            }
            else
            {
                printf("%s\n", tick);
            }
        }
        else
        {
            if (crack)
            {
                printf("A match could not be found\n");
            }
        }
    
    }


    
    if(my_rank==0)
    {
        time (&end);
        dif = difftime(end, start);
        
        if (crack)
        {
            printf("There were %d combinations checked.\n", counter);
            printf("The program ran for: %.0lf seconds.\n", dif);
            printf("Processed: %.0f combinations per second.\n", (counter/dif));
        }
    
    }
    /* Shut down MPI */
    MPI_Finalize();
    return 0;

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
    
    if( word[strlen(word)-1] == '\n' )
        word[strlen(word)-1] = '\0';
    
    if( word[strlen(word)-1] == '\r' )
        word[strlen(word)-1] = '\0';
    

    if (checkPw(word) != 0)
    {

       /* pw = word;*/
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


int crack_seq(char *guess, int *char_set, int size, int position)
{
    int i;
    if (position == (size - 1))
    {
        for(i = 0; i < (CHAR_SET_SIZE - 1); i++)
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
        for(i = 0; i < (CHAR_SET_SIZE - 1); i++)
        {
            guess[position] = (char)char_set[i];
            result = crack_seq(guess, char_set, size, (position + 1));
            if (result == 0)
            {
                return 0;
            }
        }
        return 1;
    }
}

int brute_force (void)
{
    int i;
    int size;
    int max_size = strlen(&password);
    
    int char_set[CHAR_SET_SIZE];
    
    for (i = 0; i < CHAR_SET_SIZE; i++)
    {
        char_set[i] = (i + 33);
    }
    
    for (size = 1; size <= max_size; size++)
    {
        if (!found)
        {
            char b_word[size + 1];
            
            for (i = 0; i < size; i++)
            {
                b_word[i] = (char)char_set[0];
                
                if (i == (size - 1))
                {
                    b_word[i + 1] = '\0';
                }
            }
            
            int crack_out;
            crack_out = crack_seq(b_word, char_set, size, 0);
            
            if (crack_out == 0)
            {
                found = true;
            }
        }
    }
    
    return 0;
}
void concatenate(char p[], char q[]) {
    int c, d;
    
    c = 0;
    
    while (p[c] != '\0') {
        c++;
    }
    
    d = 0;
    
    while (q[d] != '\0') {
        p[c] = q[d];
        d++;
        c++;
    }
    
    p[c] = '\0';
}
