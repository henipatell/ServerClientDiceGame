/*Advanced System Programming

Final Project - A client and a server, to implement a simple online game 

Team Members::
Henikumari Patel (105160979)
Kinjalben Dhimmar (105221393)

*/

// SERVER 
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h> // includes a number of definitions of structures needed for sockets
#include <sys/types.h> // contains definitions of a number of data types used in system calls
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <arpa/inet.h> // for htonl() and htons() functions 


void error(char *errorMsg){ //  showing error
	perror(errorMsg);
    exit(1);
}

void servicePlayers(int, int, int);

int main(int argc, char *argv[]){  
	int socket_fd, client, clientLen, portNumber; // socket_fd = file descriptor, portNumber = port number of server
	struct sockaddr_in servAdd;  // IPv4 socket address, server
	socklen_t len;
	int gamePlayer[2];
	int gameNumber=0;
	
	// server socket address
	if(argc != 2){ // user is passing the port number of server
		printf("Call model: %s <Port Number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// creating an endpoint 
	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0))<0){ // SOCK_STREAM = read in a continuous stream 
		fprintf(stderr, "Could not create socket\n"); 
		error("Could not create socket"); 
	}
	// configuring server address 
	servAdd.sin_family = AF_INET; // AF_INET = Adress family for internet
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);//servAdd = IP address of the host // htonl() converts host long byte order to network byte order
	sscanf(argv[1], "%d", &portNumber); 
	servAdd.sin_port = htons((uint16_t)portNumber); //converts short integer hostshort from host byte order to network byte order
	len = sizeof(servAdd);
	
	if (bind(socket_fd, (struct sockaddr *) &servAdd,
              sizeof(servAdd)) < 0) 
              error("ERROR on binding");// binds socket to an address
	
	if(listen(socket_fd, 5)){
		error("Listen");
	} // server starts listening, 5 is the maximum size allowed by most of the systems and it is the number of connections that can be waiting 
	
	printf("Server started and waiting for players(clients)\n");
	
	while(1){
		if((gamePlayer[0] = accept(socket_fd, (struct sockaddr *)&servAdd, (socklen_t*)&len))<0){
			error("Accept");
		}
		printf("Got player1: TOTO\n");
		printf("Waiting for player2\n"); 

		if((gamePlayer[1] = accept(socket_fd, (struct sockaddr *)&servAdd, (socklen_t*)&len))<0){
			error("Accept");
		}
		printf("Got player2: TITI \n");
		gameNumber ++;
		printf("=================Game %d Started======================\n", gameNumber);
		//printf("Now I am starting the Game %d: \n", gameNumber);
		if(!fork()){
			servicePlayers(gameNumber, gamePlayer[0], gamePlayer[1]);
			exit(0);
		}
	}
	return 0;
}

// The server’s child process being a referee for the two clients(players) 
void servicePlayers(int gameNumber, int player1Sfd, int player2Sfd){
	char score1[15], score2[15];
	int player1ScoreTotal = 0, player2ScoreTotal = 0, n;

	char *startSignal = "You can now play";
    char *winnerSignal = "Game over: you won the game";
    char *loserSignal = "Game over: you lost the game";

	printf("Being a referee, I am starting game-%d for TOTO and TITI \n", gameNumber);
	
	while(1){
		// player 1
		//printf("TOTO playing \n");
		sleep(1);
		write(player1Sfd, startSignal, strlen(startSignal)+1); // TOTO
		// get score of player 1 and count total
		n = read(player1Sfd, score1, 15);
		player1ScoreTotal += atoi(score1);
		printf("GAME::::::::%d:::::::::::   points of TOTO :%d \n",gameNumber, player1ScoreTotal);
		if (player1ScoreTotal >= 100){
			// Acknowlege winner
			write(player1Sfd, winnerSignal, strlen(winnerSignal)+1);
			// Acknowlege loser
			write(player2Sfd, loserSignal, strlen(loserSignal)+1);
			//finish current game
			printf("Winner is TOTO from GAME-%d \n", gameNumber);
			printf("=================Game %d Over======================\n", gameNumber);
			// close both sockets
			close(player1Sfd);
			close(player2Sfd);
			exit(0);
		}
        sleep(1);
		// player 2
		//printf("TITI playing \n");
		write(player2Sfd, startSignal, strlen(startSignal)+1); // TITI
	
		// get score of player 2 and count total
		n = read(player2Sfd, score2, 15); 
		player2ScoreTotal += atoi(score2);
		printf("GAME::::::::%d:::::::::::   points of TITI :%d \n",gameNumber, player2ScoreTotal);
		if (player2ScoreTotal >= 100){
			//Acknowlege winner
			write(player2Sfd, winnerSignal, strlen(winnerSignal)+1);
			//Acknowlege loser
			write(player1Sfd, loserSignal, strlen(loserSignal)+1);
			// finish current game
			printf("Winner is TITI from GAME-%d \n", gameNumber);
			printf("=================Game %d Over======================\n", gameNumber);
			// close both sockets
			close(player1Sfd);
			close(player2Sfd);
			exit(0);
		}
	}			
}

// void finishGame(int player1Sfd, int player2Sfd, int gameNumber){
// 	printf("=================Game %d Over======================\n", gameNumber);
// 	// close both sockets
// 	close(player1Sfd);
// 	close(player2Sfd);
// 	exit(0);
// }


