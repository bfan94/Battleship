/**
 * net.cpp
 * Networking and communications routines.
 * John Greenslade, Billy Fanelli
 **/
#include "net.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "GameCore.h"

// Fuck boost, I'm just doing raw sockets. --S

int sockfd, newsockfd;
char buffer[8];
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
struct hostent *server;
int n;
#define PORTNO 50405

void serverStart(void)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) std::cout << "ERROR opening socket" << std::endl; // Exception?

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORTNO);

    if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
	{
		std::cout << "Error on binding to net adapter." << std::endl;
	}
    listen(sockfd,5);

    clilen = sizeof(cli_addr);
	// This next line blocks until a connection is made.
    newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (newsockfd < 0)
	{
		std::cout << "ERROR on accept" << std::endl;
	}
    bzero(buffer,8);
}

void clientStart(char* addr)
{
	newsockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (newsockfd < 0) std::cout << "ERROR opening socket" << std::endl; // Exception?

	server = gethostbyname(addr);
	if( server == NULL ) std::cout << "ERROR validating host" << std::endl; // Exception?

	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	 (char *)&serv_addr.sin_addr.s_addr,
	  server->h_length);
	serv_addr.sin_port = htons(PORTNO);

	if (connect(newsockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		std::cout << "Error on connecting to remote." << std::endl;
	}
}

bool moveSend(int move)
{
    bzero(buffer,8);

	// Send coords.
	//itoa(move, buffer, 10);
	sprintf(buffer, "%d", move);
	n = write(newsockfd,buffer,2);
	if (n < 0) std::cout << "ERROR writing to socket" << std::endl;

	// Receive response.
    bzero(buffer,8);
    n = read(newsockfd,buffer,4); // ALL THE ASSUMPTIONS, NONE OF THE ERROR CHECKING! #YOLO

	if( 0 == strncmp(buffer, "HIT ", 4) )
	{
		gridElem* i = &(eg->mListStart);
		while( (*i).next ) i = (*i).next;
		(*i).m = new Missile((move/10),(move%10)); // Extracting digits the tricky way.
		(*i).next = new gridElem;
		(*i).m->setHit(true);
		return true;
	}
	else if( 0 == strncmp(buffer, "MISS", 4) )
	{
		gridElem* i = &(eg->mListStart);
		while( (*i).next ) i = (*i).next;
		(*i).m = new Missile((move/10),(move%10)); // Extracting digits the tricky way.
		(*i).next = new gridElem;
		(*i).m->setHit(false);
		return false;
	}
	else if( 0 == strncmp(buffer, "DEAD", 4) )
	{
		return true;
		gameOver = true;
	}
	else std::cout << "error: invalid response." << std::endl; // Actually, this is kind of error checking.
	return false;

}

bool moveReceive(void)
{
    bzero(buffer,8);

	// Get coords.
    n = read(newsockfd,buffer,2); // ALL THE ASSUMPTIONS, NONE OF THE ERROR CHECKING! #YOLO
    if (n < 0) std::cout << "ERROR reading from socket" << std::endl;
	int mov = atoi(buffer);

	// Insert hit.
	gridElem* i = &fg->mListStart;
	while( (*i).next ) i = (*i).next;
	(*i).m = new Missile((mov/10),(mov%10)); // Extracting digits the tricky way.
	(*i).next = new gridElem;

	// Determine hit type.
	if( s1->checkValidHit((mov/10), (mov%10)) ||
	    s2->checkValidHit((mov/10), (mov%10)) ||
	    s3->checkValidHit((mov/10), (mov%10)) ||
	    s4->checkValidHit((mov/10), (mov%10)) ||
	    s5->checkValidHit((mov/10), (mov%10)) )
	{
		(*i).m->setHit(true);
		hitCount++;
		if(hitCount >= (5+4+3+3+2))
		{
			n = write(newsockfd,"DEAD",4);
			if (n < 0) std::cout << "ERROR writing to socket" << std::endl;
			gameOver = true;
			return true;
		}
		else
		{
			n = write(newsockfd,"HIT ",4);
			if (n < 0) std::cout << "ERROR writing to socket" << std::endl;
			return true;
		}
	}
	else
	{
		(*i).m->setHit(false);
		n = write(newsockfd,"MISS",4);
		if (n < 0) std::cout << "ERROR writing to socket" << std::endl;
		return false;
	}
}

void serverEnd(void)
{
    close(newsockfd);
    close(sockfd);
}
