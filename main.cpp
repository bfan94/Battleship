/**
 * main.cpp
 * Overall program flow & socket stuff.
 * John Greenslade, Billy Fanelli
 **/

#include <iostream>
#include <thread>
#include "GameCore.h"
#include "TUI.h"
#include "net.h"

FriendGrid *fg;
EnemyGrid *eg;

Ship* s1;
Ship* s2;
Ship* s3;
Ship* s4;
Ship* s5;

bool gameOver;

int hitCount = 0;

int colConvert(char col)
{
	switch(col)
	{
		case 'A': case 'a':
			return 0;
		case 'B': case 'b':
			return 1;
		case 'C': case 'c':
			return 2;
		case 'D': case 'd':
			return 3;
		case 'E': case 'e':
			return 4;
		case 'F': case 'f':
			return 5;
		case 'G': case 'g':
			return 6;
		case 'H': case 'h':
			return 7;
		case 'I': case 'i':
			return 8;
		case 'L': case 'l':
			return 9;
		default:
			return -1; // Error.
	}
}

int main(int argc, char *argv[])
{
	bool server = true; 

	if(argc == 2) // Assumed IP/host as argument.
	{
		std::cout << "Connecting to host...";
		std::cout.flush();
		clientStart(argv[1]);
		std::cout << "Done!" << std::endl;
		server = false;
	}
	else
	{
		std::cout << "Waiting for client...";
		std::cout.flush();
		serverStart();
		std::cout << "Done!" << std::endl;
	}
	
	// Coordinate inputs.
	char col, orien;
	int row, colFix;
	bool retry;

	// Initialize grids. The names mean nothing.
	fg = new FriendGrid("Timmothy");
	eg = new EnemyGrid("Chris");

	// Draw empty grid
	fg->print();

	// Begin ship entry routines.
	retry = true;
	while(retry)
	{

		// Prompt for 1st ship
		std::cout << "Placing first ship: Carrier, length 5." << std::endl
				  << "Type coordinates and orientation (h/v), space separated (e.g. B 4 h), then hit enter: ";
		std::cin >> col >> row >> orien;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);
		if(orien == 'h' || orien == 'H') 
		{
			s1 = new Carrier(colFix, row, false);
			if(s1->checkValidPos(colFix, row, false))
			{
				retry = false;
			}
			else
			{
				delete s1;
				// Draw empty grid
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else if(orien == 'v' || orien == 'V')
		{
			s1 = new Carrier(colFix, row, true);
			if(s1->checkValidPos(colFix, row, true))
			{
				retry = false;
			}
			else
			{
				delete s1;
				// Draw empty grid
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else
		{
			fg->print();
			std::cout << "Error: invalid orientation selected. Please try again." << std::endl;
		}
	}
	fg->placeShip(s1, 1);
	fg->print();

	// Ship 2
	retry = true;
	while(retry)
	{

		// Prompt for 2nd ship
		std::cout << "Placing second ship: Battleship, length 4." << std::endl
				  << "Type coordinates and orientation (h/v), space separated (e.g. B 4 h), then hit enter: ";
		std::cin >> col >> row >> orien;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);
		if(orien == 'h' || orien == 'H') 
		{
			s2 = new Battleship(colFix, row, false);
			if(s2->checkValidPos(colFix, row, false))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix+1, row) ||
					s1->checkValidHit(colFix+2, row) ||
					s1->checkValidHit(colFix+3, row) ) 
				{
					delete s2;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s2;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else if(orien == 'v' || orien == 'V')
		{
			s2 = new Battleship(colFix, row, true);
			if(s2->checkValidPos(colFix, row, true))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix, row+1) ||
					s1->checkValidHit(colFix, row+2) ||
					s1->checkValidHit(colFix, row+3) )
				{
					delete s2;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s2;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else
		{
			fg->print();
			std::cout << "Error: invalid orientation selected. Please try again." << std::endl;
		}
	}
	fg->placeShip(s2, 2);
	fg->print();

	// Ship 3
	retry = true;
	while(retry)
	{

		// Prompt for 3rd ship
		std::cout << "Placing third ship: Submarine, length 3." << std::endl
				  << "Type coordinates and orientation (h/v), space separated (e.g. B 4 h), then hit enter: ";
		std::cin >> col >> row >> orien;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);
		if(orien == 'h' || orien == 'H') 
		{
			s3 = new Submarine(colFix, row, false);
			if(s3->checkValidPos(colFix, row, false))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix+1, row) ||
					s1->checkValidHit(colFix+2, row) ||
					s2->checkValidHit(colFix+0, row) ||
					s2->checkValidHit(colFix+1, row) ||
					s2->checkValidHit(colFix+2, row) ) 
				{
					delete s3;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s3;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else if(orien == 'v' || orien == 'V')
		{
			s3 = new Submarine(colFix, row, true);
			if(s3->checkValidPos(colFix, row, true))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix, row+1) ||
					s1->checkValidHit(colFix, row+2) ||
					s2->checkValidHit(colFix, row+0) ||
					s2->checkValidHit(colFix, row+1) ||
					s2->checkValidHit(colFix, row+2) )
				{
					delete s3;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s3;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else
		{
			fg->print();
			std::cout << "Error: invalid orientation selected. Please try again." << std::endl;
		}
	}
	fg->placeShip(s3, 3);
	fg->print();

	// Ship 4
	retry = true;
	while(retry)
	{

		// Prompt for 4th ship
		std::cout << "Placing fourth ship: Destroyer, length 3." << std::endl
				  << "Type coordinates and orientation (h/v), space separated (e.g. B 4 h), then hit enter: ";
		std::cin >> col >> row >> orien;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);
		if(orien == 'h' || orien == 'H') 
		{
			s4 = new Destroyer(colFix, row, false);
			if(s4->checkValidPos(colFix, row, false))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix+1, row) ||
					s1->checkValidHit(colFix+2, row) ||
					s2->checkValidHit(colFix+0, row) ||
					s2->checkValidHit(colFix+1, row) ||
					s2->checkValidHit(colFix+2, row) ||
					s3->checkValidHit(colFix+0, row) ||
					s3->checkValidHit(colFix+1, row) ||
					s3->checkValidHit(colFix+2, row) ) 
				{
					delete s4;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s4;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else if(orien == 'v' || orien == 'V')
		{
			s4 = new Destroyer(colFix, row, true);
			if(s4->checkValidPos(colFix, row, true))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix, row+1) ||
					s1->checkValidHit(colFix, row+2) ||
					s2->checkValidHit(colFix, row+0) ||
					s2->checkValidHit(colFix, row+1) ||
					s2->checkValidHit(colFix, row+2) ||
					s3->checkValidHit(colFix, row+0) ||
					s3->checkValidHit(colFix, row+1) ||
					s3->checkValidHit(colFix, row+2) )
				{
					delete s4;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s4;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else
		{
			fg->print();
			std::cout << "Error: invalid orientation selected. Please try again." << std::endl;
		}
	}
	fg->placeShip(s4, 4);
	fg->print();

	// Ship 5
	retry = true;
	while(retry)
	{

		// Prompt for 5th ship
		std::cout << "Placing fifth ship: Patrol Boat, length 2." << std::endl
				  << "Type coordinates and orientation (h/v), space separated (e.g. B 4 h), then hit enter: ";
		std::cin >> col >> row >> orien;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);
		if(orien == 'h' || orien == 'H') 
		{
			s5 = new PatrolBoat(colFix, row, false);
			if(s5->checkValidPos(colFix, row, false))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix+1, row) ||
					s2->checkValidHit(colFix+0, row) ||
					s2->checkValidHit(colFix+1, row) ||
					s3->checkValidHit(colFix+0, row) ||
					s3->checkValidHit(colFix+1, row) ||
					s4->checkValidHit(colFix+0, row) ||
					s4->checkValidHit(colFix+1, row) ) 
				{
					delete s5;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s5;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else if(orien == 'v' || orien == 'V')
		{
			s5 = new PatrolBoat(colFix, row, true);
			if(s5->checkValidPos(colFix, row, true))
			{
				if( s1->checkValidHit(colFix, row) ||
					s1->checkValidHit(colFix, row+1) ||
					s2->checkValidHit(colFix, row+0) ||
					s2->checkValidHit(colFix, row+1) ||
					s3->checkValidHit(colFix, row+0) ||
					s3->checkValidHit(colFix, row+1) ||
					s4->checkValidHit(colFix, row+0) ||
					s4->checkValidHit(colFix, row+1) )
				{
					delete s5;
					fg->print();
					std::cout << "Error: invalid position selected. Please try again." << std::endl;
				}
				else retry = false; // Continue, ship OK.
			}
			else
			{
				delete s5;
				fg->print();
				std::cout << "Error: invalid position selected. Please try again." << std::endl;
			}
		}
		else
		{
			fg->print();
			std::cout << "Error: invalid orientation selected. Please try again." << std::endl;
		}
	}
	fg->placeShip(s5, 5);
	fg->print();

	std::cout << "Grid set." << std::endl;
	gameOver = false;

	if(server)
	{
		// Player move.
		eg->print();
		std::cout << "It is your move. Specify a location, space separated (e.g B 4), then hit enter: ";
		std::cin >> col >> row;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);

		if ( moveSend((colFix*10)+row) ) // Move int-ifyed for socket transmission.
		{
			eg->print();
			std::cout << "Hit!";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		else
		{
			eg->print();
			std::cout << "Miss!";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}

	}
	while(!gameOver) // Check.
	{
		// Enemy move.
		fg->print();
		std::cout << "Waiting for enemy move...";

		if( moveReceive() )
		{
			fg->print();
			std::cout << "Hit!";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		else
		{
			fg->print();
			std::cout << "Miss!";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		
		// Check.
		if(gameOver) break;

		// Player move.
		eg->print();
		std::cout << "It is your move. Specify a location, space separated (e.g B 4), then hit enter: ";
		std::cin >> col >> row;
		row -= 1; // Convert to 0-indexed.
		colFix = colConvert(col);

		if ( moveSend((colFix*10)+row) ) // Move int-ifyed for socket transmission.
		{
			std::cout << "Hit!";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(4));
		}
		else
		{
			std::cout << "Miss!";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::seconds(4));
		}
	}

	std::cout << "Game over!";

	serverEnd();
	// TESTING STUFF

	//fg->mListStart.m = new Missile(1,1);
	//fg->mListStart.next = new gridElem;

	return(0);
}
