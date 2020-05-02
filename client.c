/*Advanced System Programming

Final Project - A client and a server, to implement a simple online game 

Team Members::
Henikumari Patel (105160979)
Kinjalben Dhimmar (105221393)

*/

// CLIENT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h> //generic address structure is called sockaddr, deﬁned here, for creating connection by calling socket()
#include <sys/types.h>  
#include <netinet/in.h> //Internet socket address structure - The structure is called sockaddr in, deﬁned here 

int main(int argc, char *argv[]){
    char message[50], score[15]; 
    int server, portNumber, n; 
    socklen_t len; 
    struct sockaddr_in servAdd;
    int dicePoints;
    char *startSignal = "You can now play";
    char *winnerSignal = "Game over: you won the game";
    char *loserSignal = "Game over: you lost the game";
    long int ss = 0;

    if(argc != 3){ 
        printf("Call model: %s <IP> <Port #>\n", argv[0]); 
        exit(0); } 
        
    if((server=socket(AF_INET, SOCK_STREAM, 0))<0){ 
        fprintf(stderr, "Cannot create socket\n"); 
        exit(1); 
    } 
    servAdd.sin_family = AF_INET; 
    sscanf(argv[2], "%d", &portNumber); 
    servAdd.sin_port = htons((uint16_t)portNumber);

    if(inet_pton(AF_INET, argv[1],&servAdd.sin_addr)<0){ // convert IPv4 and IPv6 addresses from text to binary form
		fprintf(stderr, " inet_pton() has failed\n"); 
		exit(2); 
	} 

	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){ 
		fprintf(stderr, "connect() has failed, exiting\n"); 
		exit(3); 
	}  
    printf("I am playing............\n");
    while(1){ /* reading server’s messages */ 
        n=read(server, message, 50);
        if((strcmp(message, startSignal)==0)){ 
            // play dice
            dicePoints = ((int)time(&ss) % 10) + 1;
            sprintf(score, "%d", dicePoints);
            printf("Dice rolled................\n");
            printf("obtained score: %s \n", score);
         
            // send message to server  
            write(server, score, strlen(score)+1);

        }else if((strcmp(message, winnerSignal)==0)){
            // player won the game
            //fprintf(stderr, "%s", "I won the game");
            printf("I won the game");
            close(server);
            exit(0);
            
        }else if((strcmp(message, loserSignal)==0)){
            // player won the game
            printf("I lost the game");
            //fprintf(stderr, "%s", "I lost the game");
            close(server);
            exit(0);
        }
        
    }
    //exit(0);
    
}
        