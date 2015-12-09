#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
/* The global variables are shared among all the threads. */
static const char alphabet[] =
"abcdefghijklmnopqrstuvwxyz"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"0123456789"
"!@#$%^&*()_+-={}|[];'<>?,./";

static const int alphabet_size = sizeof(alphabet) - 1;

char  password ;
char *  ticker;

int     thread_count;
int method;
/* critical section lock variables */
int flag;
sem_t sem;
pthread_mutex_t mutex;
double  total;

char getPassword(char* pass);
int checkPW(char* str);
void crack(char* str, int index, int depth);
void crack_seq(int max_length);

void *Thread_work(void* rank);


/*--------------------------------------------------------------*/
int main(int argc, char** argv) {
    long i;
    pthread_t*  thread_handles;
    
    total = 0.0;
    if (argc != 3) {
        fprintf(stderr, "usage: %s <number of threads> <method>\n",
                argv[0]);
        exit(0);
    }
    thread_count = strtol(argv[1], NULL, 10);
    
    method = strtol(argv[2], NULL, 10);
    
    printf("Enter password\n");
    scanf("%s",&password);

    thread_handles = malloc (thread_count*sizeof(pthread_t));
    sem_init(&sem, 0, 1);

    for (i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, thread_crack,
                       (void*) i);
    }
    
    for (i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }
    
    sem_destroy(&sem);

    return 0;
} /*  main  */

/*--------------------------------------------------------------*/
void *thread_crack(void* rank) {
 
    long my_rank = (long) rank;
    
    int pass_length = strlen(&password);
    pass_length / thread_count;
    
    sem_wait(&sem);
    crack_seq(pass_length);
    sem_post(&sem);
        
}  /* Thread_work */

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
        ticker = &password;
        a++;
        
    }
    
    return a ;
}
void crack(char * str, int index, int max_depth)
{
    int i;
    
    for (i = 0; i < alphabet_size; ++i)
    {
        str[index] = alphabet[i];
        
        if (index == max_depth - 1 )
        {
            if(checkPw(str) == 1)
            {
                printf("Success! Guess = %s\nPassword = %s\n", str , &password);
                ticker = &password;
                
            }
        }
        else
        {
            crack(str, index + 1, max_depth);
        }
    }
}

void crack_seq(int max_len)
{
    char * buf = malloc(max_len + 1);
    int i;
    
    for (i = 1; i <= max_len; ++i)
    {
        memset(buf, 0, max_len + 1);
        crack(buf, 0, i);
    }
    
    free(buf);
}


