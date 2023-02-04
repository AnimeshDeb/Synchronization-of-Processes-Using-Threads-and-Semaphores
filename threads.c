#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "stdbool.h"

//Below boolean is responsible for the continuation of the while loop.
bool loop_continuer = true;
// First thing we need to do is initialize the pthread mutexes. This is simply done by initializing the mutexes, named mainmut, agent_mut, match_mut, paper_mut, and tobacco_mut using
// PTHREAD_MUTEX_INITIALIZER. In addition, the mutexes were also initialized by giving them a type of pthread_mutex_t.
pthread_mutex_t mainmut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agent_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t match_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t paper_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tobacco_mut = PTHREAD_MUTEX_INITIALIZER;
// After initializing the mutexes, we have to create the various threads that the program will need. The threads that are created are shown below.
pthread_t AT;
pthread_t MT;
pthread_t PT;
pthread_t TT;
// Decided to do the overal program in terms of functions. The function below is for the smokers.
void *tds(void *ptr)
{
    // Start of by creating a pointer and then changing its type to an integer.
    const char *whtch = (char *)ptr;
    int changer = atoi(whtch);
    // loop below will continue while loop_continuer is true.
    while (loop_continuer)
    {
        // Decided to relate the number 1 to the smoker that has paper. If the changer variable stores the value of 1, then the code below in the first if condition/ statement will run.
        // Using pthread_mutex_lock, we acquire the ownership of the mutex that is specified in the paranthesis. Following this, a printf is used to let the user know that the smoker
        // with the paper got all the items. Then pthread_mutex_unlock is used so that other threads can have access to the below code. Then a final printf is used to let the user know
        // that the smoker with the paper is smoking.
        if (changer == 1)
        {
            pthread_mutex_lock(&paper_mut);
            pthread_mutex_lock(&mainmut);
            printf("The smoker with the paper has gotten the other items required to smoke, tobacco and match.\n");
            pthread_mutex_unlock(&agent_mut);
            pthread_mutex_unlock(&mainmut);
            printf("The smoker with the paper is right now smoking succesfully.\n");
        }
        // Decided to relate the number 2 to the smoker that has match. If the changer variable stores the value of 2, then the code below in the second if condition/ statement will run.
        // Using pthread_mutex_lock, we acquire the ownership of the mutex that is specified in the paranthesis. Following this, a printf is used to let the user know that the smoker
        // with the match got all the items required to smoke. Then pthread_mutex_unlock is used so that other threads can have access to the below code. Then a final printf is used to let the user know
        // that the smoker with the match is smoking.
        if (changer == 2)
        { 

            pthread_mutex_lock(&match_mut); 
            pthread_mutex_lock(&mainmut);
            printf("The smoker with the match has gotten the other items required to smoke, tobacco and paper.\n");
            pthread_mutex_unlock(&agent_mut);
            pthread_mutex_unlock(&mainmut);
            printf("The smoker with match is right now smoking successfully.\n");
        }
        // Decided to relate the number 3 to the smoker that has tobacco. If the changer variable stores the value of 3, then the code below in this last if condition/ statement will run.
        // Using pthread_mutex_lock, we acquire the ownership of the mutex that is specified in the paranthesis. Following this, a printf is used to let the user know that the smoker
        // with the tobacco got all the items that are required to smoke. Then pthread_mutex_unlock is used so that other threads can have access to the below code. Then a final printf is used to let the user know
        // that the smoker with the tobacco is smoking.
        if (changer == 3)
        { 
            pthread_mutex_lock(&tobacco_mut);
            pthread_mutex_lock(&mainmut);
            printf("The smoker with the tobacco has gotten the other items required to smoke, match and paper.\n");
            pthread_mutex_unlock(&agent_mut);
            pthread_mutex_unlock(&mainmut);
            printf("The smoker with tobacco is right now smoking successfully.\n");
        }
    }
}

//Function that will handle the agent part of the program.
void *ap()
{

    //below for loop is used to control how many times the agent places the items on the table. The loop will continue for 30 times as noted below.
    for (int L = 0; L < 30; L++)
    {
        pthread_mutex_lock(&mainmut);
        //Using rand() to choose a random number from 1,2, and 3.
        int determiner = rand() % 3 + 1;
        //If 1 is the random number that's chosen, then the below code in the fist if statement will run. In this, the agent places the rquired items, with respect to the smoker that already has 
        //paper, on the table so that the paper smoker can get all the items needed to smoke.
        if (determiner == 1)
        { 
            printf("The agent places the items match and tobacco on the table so that the smoker with the paper can take them.\n");
            pthread_mutex_unlock(&paper_mut);
        }
        //If 2 is the random number that's chosen, then the below code in the second if statement will run. In this, the agent places the rquired items, with respect to the smoker that already has 
        //match, on the table so that the match smoker can get all the items needed to smoke.
        if (determiner == 2)
        { 
            printf("The agent places the items paper and tobacco on the table so that the smoker with match can take them.\n");
            pthread_mutex_unlock(&match_mut);
        }
        //If 3 is the random number that's chosen, then the below code in this last if statement will run. In this, the agent places the rquired items, with respect to the smoker that already has 
        //tobacco, on the table so that the tobacco smoker can get all the items needed to smoke.
        else if (determiner == 3)
        { 
            printf("The agent places the items paper and macth on the table so that the smoker with the tobacco can take them.\n");
            pthread_mutex_unlock(&tobacco_mut);
        }
        pthread_mutex_unlock(&mainmut);
        pthread_mutex_lock(&agent_mut);
    }
    pthread_mutex_lock(&mainmut);
    //Using the pthread_cancel, we finish the threads that are specified below.
    pthread_cancel(TT); 
    pthread_cancel(PT); 
    pthread_cancel(MT); 
    pthread_mutex_unlock(&mainmut);
}

//main function that calls the other two functions and makes the entire program work.
int main()
{
    //srand is used for randomization.
    srand(time(0));
    //pthread_mutex_lock is used to initialize the mutexes.
    pthread_mutex_lock(&agent_mut);
    pthread_mutex_lock(&paper_mut);
    pthread_mutex_lock(&tobacco_mut);
    pthread_mutex_lock(&match_mut);
    //Below code refers to the paper, match, and the tobacco smokers.
    char *SP = "1";
    char *SM = "2";   
    char *ST = "3"; 
    //Using pthread_create, the threads are being created.
    pthread_create(&AT, NULL, ap, NULL);
    pthread_create(&MT, NULL, tds, (void *)SM);
    pthread_create(&PT, NULL, tds, (void *)SP);
    pthread_create(&TT, NULL, tds, (void *)ST);

    //Below code finishes the agent thread.
    pthread_join(AT, NULL);
    exit(0);
}