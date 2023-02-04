#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include "sem.h"

int main(){
	//Below code segment just initializes variables that will be used throughout the entire program. 
	int p;
	int q; 
	int r; 
	int s;
	int bound = 30; 
	//srand is used below for randomization.
    srand(time(0)); 
    //semget is used to initialize the verious sempahores that will be used in the program below.
	int smokermatch = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smokerpaper = semget(IPC_PRIVATE,1, 0666 | IPC_CREAT);
    int mainsem = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int agentsem = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    int smokertobacco= semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    //sem_create is used to actually create the semaphores. For mainsem, one of parameters is 1 so that the critical section is protected. For the other semaphores, 0 is used because the
	//P() and V() semaphore functions will be used.
	sem_create(smokermatch,0);
    sem_create(smokerpaper,0);
    sem_create(mainsem,1); 
    sem_create(agentsem,0);
    sem_create(smokertobacco,0);
	//fork() is used to create a child process, named smoker_match. The if statements below serve to check if the forking is successful.
    pid_t smoker_match = fork(); 
	if(smoker_match==-1)
	{
		printf("The forking is not sucessful!\n");
		perror("fork");	
	}
	//In the child process, smoker_match, the smoker with match is concerned. P() is used to lock the semaphores smokermatch and mainsem. Then using the printf, it is indicated that 
	// the smoker with match gets the tobacco and paper from the agent. V() is used to unlock the mentioned semaphores. Final printf is used to show that the smoker with match has already
	// gotten the tobbacco and paper and is smoking.
    if(smoker_match == 0)
    {
        for(p = 0; p < bound; p++){
            P(smokermatch);
            P(mainsem);
            printf("The smoker with the match has gotten the other items required to smoke, tobacco and paper. \n");
            V(agentsem);
            V(mainsem);
            printf("The smoker right now is smoking successfully.\n\n");
        }
    }
	//The code segment below creates a variable of type pid_t and fork() is used to create another child. The if statements below checks to see if the forking is successful or not.
	//If it is not successful then an appropriete error message will be outputted.
    pid_t smoker_paper  = fork(); 
    if(smoker_paper ==-1)
    {
		printf("The forking is not successful!\n");
        perror("fork");
    }
	//The code segment below starts of with a for loop. In the for loop, P() is used to lock the semaphores smokerpaper and mainsem. Then a printf is used to indicate to the user that the 
	//smoker with the paper gets the tobacco and match from the agent. V() is used to unlock the mentioned semaphores. The last printf demonstrates that the smoker with the paper has already 
	//gotten all the required items to smoke.
    if(smoker_paper == 0)
    {
        for(q = 0; q < bound; q++){
            P(smokerpaper);
            P(mainsem);
            printf("The smoker with the paper has gotten the other items required to smoke, tobacco and match.\n");
            V(agentsem);
            V(mainsem);
            printf("The smoker with the paper right now is smoking successfully.\n\n");
        }
    }
	//The code segment below creates a variable of type pid_t and fork() is used to create another child. The if statements below checks to see if the forking is successful or not.
	//If it is not successful then an appropriete error message will be outputted.
    pid_t smoker_tobacco = fork();
    if(smoker_tobacco == -1)
    {
        printf("The forking is not successful!\n");
        perror("fork");
    
    }
	//The code segment below starts of with the P() in a for loop. The P() locks the semaphores smokertobacco and mainsem. After, a printf is used to indicate that the smoker with the tobacco 
	// gets the match and paper from the agent. V() is used to unclock the mentioned semaphors and the last printf illustrates that the smoker with the paper has already gotten all the 
	//required items to smoke.
    if(smoker_tobacco == 0)
    {
        for(r = 0; r < bound; r++){
            P(smokertobacco);
            P(mainsem);
            printf("The smoker with the tobacco has gotten the other items required to smoke, match and paper.\n");
            V(agentsem);
            V(mainsem);
            printf("The smoker with the tobacco right now is smoking successfully.\n\n");
        }
    }
    //The code below concerns the agent process. Depending on the randomization, a number is retrieved. Based on this number, agent determines which item to pass to which smoker.
    if(fork() == 0)
    { 
        for (s = 0; s < bound; s++){
            P(mainsem);
            //rand() is used to choose a random integer between 1,2,and 3
            int determiner = rand() % 3+1; 
            //Decided to relate the number 1 to the smoker with the match. If 1 is chosen randomly, then the agent will place paper and tobacco onto the table for the smoker with the match to take.
            if (determiner == 1){ 
                printf("The agent places the items paper and tobacco on the table so that the smoker with match can take them.\n");
                V(smokermatch);
            }
            //Decided to relate the number 2 to the smoker with the paper. If 2 is chosen randomly, then the agent will place match and tobacco onto the table for the smoker with the paper to take.
            else if (determiner == 2){ 
                printf("The agent places the items match and tobacco on the table so that the smoker with the paper can take them.\n");
                V(smokerpaper);
            }
            //Decided to relate the number 3 to the smoker with the tobacco. If 3 is chosen randomly, then the agent will place paper and match onto the table for the smoker with the tobacco to take.
            else if (determiner == 3){ 
                printf("The agent places the items paper and macth on the table so that the smoker with the tobacco can take them.\n");
                V(smokertobacco);
            }
            V(mainsem);
            P(agentsem);
        }
        //kill is used to eliminate and kill the semaphores as we won't need to use them anymore.
        kill(smokerpaper, SIGTERM);
        kill(mainsem, SIGTERM);
        kill(smokertobacco, SIGTERM);
        kill(smokermatch, SIGTERM);
        kill(agentsem, SIGTERM);
    }
    //wait is used with respect to the parent process.
    wait(0);
    return 0;
}