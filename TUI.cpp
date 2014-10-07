/**
 * TUI.cpp
 * Main graphics and interface routines.
 * John Greenslade, Billy Fanelli
 **/

#include "TUI.h"
#include <iostream>

void ClearScreen(void)
{
	if (!cur_term)
	{
		int result;
		setupterm( NULL, STDOUT_FILENO, &result );
		if (result <= 0) return;
	}
	putp( tigetstr( "clear" ) );
}

void EnemyGrid::print(void)
{
	UInode start; // Create the first node in the list, which defaults to the end sentinel.
	start.next = new UInode; // Lazy hack to prevent segfault on edge case.
	int newX, newY; // Some temps to make the linked list math more readable.
	UInode* j;
	UInode* tmp;
	// No Phase 1, no known ship locations.

	// Phase 2: insert missiles.

	gridElem *p = &mListStart; // Start at the beginning of the missile list. Last (invalid) missile will point null.
	while( (*p).next )
	{
		// Go through the grid list to find the cell to insert / overwrite the missile in.
		newX = (*p).m->getPosX();
		newY = (*p).m->getPosY();
		j = &start;
		if( ((*j).y*10)+(*j).x == ((newY)*10+(newX)) ) // Short circuit check for first node.
		{
			(*j).l = "X";
			(*j).r = "X";
		}
		else // Normal checks.
		{
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			if( ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) == ((newY)*10+(newX)) ) // Same position, missile overwrites ship segment.
			{
				(*((*j).next)).l = "X";
				(*((*j).next)).r = "X";
			}
			else // Miss, insert new node.
			{
				tmp = new UInode; // Make a new node to insert into the list.
				(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
				(*j).next = tmp; // And point the previous node to the new node.

				(*tmp).x = newX;
				(*tmp).y = newY;
				(*tmp).l = ".";
				(*tmp).r = '.';
			}
		}
		// And advance to the next missile.
		p = (*p).next;
	}

	// Phase 3 - Print the grid.
	j = &start;
	ClearScreen();
	// Draw top of game grid.
	std::cout << "            ENEMY GRID:" << std::endl <<
	   		     "   A  B  C  D  E  F  G  H  I  L" << std::endl <<
				 "  +--+--+--+--+--+--+--+--+--+--+" << std::endl;
	int i, k;
	for(i = 0; i < 10; i++) // Row iterator.
	{
		std::cout << boost::format("%2d") % (i+1) << "|"; // Print current row index and cell preamble.
		for(k = 0; k < 10; k++) // Column iterator.
		{
			if( (*j).x == k && (*j).y == i )
			{
				std::cout << (*j).l << (*j).r << "|";
				if( (*j).next ) j = (*j).next;
			}
			else std::cout << "  |"; // Empty cell.
		}
		std::cout << std::endl << "  +--+--+--+--+--+--+--+--+--+--+" << std::endl;
	}
}

// Routine to draw a gameboard on the screen.
void FriendGrid::print(void)
{
	// Phase 1 - Assemble ship data.
	UInode start; // Create the first node in the list, which defaults to the end sentinel.
	UInode* nPtr;
	// Go through all 5 ships.
	// S1
	if(ship1)
	{
		if(ship1->getOrient()) // Ship 1 is vertical
		{ // Carrier, length 5.
			start.x = ship1->getPosX();
			start.y = ship1->getPosY();
			start.l = "┏";
			start.r = "┓";
			start.next = new UInode;
			nPtr = start.next;

			for(int i = 0; i < 3; i++)
			{
				(*nPtr).x = ship1->getPosX();
				(*nPtr).y = ship1->getPosY() + i + 1;
				(*nPtr).l = "┃";
				(*nPtr).r = "┃";
				(*nPtr).next = new UInode;
				nPtr = (*nPtr).next;
			}

			(*nPtr).x = ship1->getPosX();
			(*nPtr).y = ship1->getPosY() + 4;
			(*nPtr).l = "┗";
			(*nPtr).r = "┛";
			(*nPtr).next = new UInode;
			nPtr = (*nPtr).next;
		}
		else // Ship 1 is horizontal
		{
			start.x = ship1->getPosX();
			start.y = ship1->getPosY();
			start.l = '[';
			start.r = "━";
			start.next = new UInode;
			nPtr = start.next;

			for(int i = 0; i < 3; i++)
			{
				(*nPtr).x = ship1->getPosX() + i + 1;
				(*nPtr).y = ship1->getPosY();
				(*nPtr).l = "━";
				(*nPtr).r = "━";
				(*nPtr).next = new UInode;
				nPtr = (*nPtr).next;
			}

			(*nPtr).x = ship1->getPosX() + 4;
			(*nPtr).y = ship1->getPosY();
			(*nPtr).l = "━";
			(*nPtr).r = ']';
			(*nPtr).next = new UInode;
			nPtr = (*nPtr).next;
		}
	}

	int newX, newY; // Some temps to make the linked list math more readable.
	UInode* j;
	UInode* tmp;

	// S2 - length 4
	if(ship2)
	{
		newX = ship2->getPosX();
		newY = ship2->getPosY();
		j = &start;
		if(ship2->getOrient()) // Vert.
		{
			// Advance until we're at the desired pos or we hit the end of the chain.
			// Target = ((newY)*10+(newX))
			// Next = ((((*((*j).next)).y) * 10) + ((*((*j).next)).x))
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode(); // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = "┏";
			(*tmp).r = "┓";

			for(int i = 0; i < 2; i++)
			{
				// Advance until we're at the desired row or we hit the end of the chain.
				while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+i+1)*10+(newX)) ) j = (*j).next;
				tmp = new UInode; // Make a new node to insert into the list.
				(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
				(*j).next = tmp; // And point the previous node to the new node.

				(*tmp).x = newX;
				(*tmp).y = newY + i + 1;
				(*tmp).l = "┃";
				(*tmp).r = "┃";
			}

			// Advance until we're at the desired row or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+3)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY + 3;
			(*tmp).l = "┗";
			(*tmp).r = "┛";
		}
		else
		{
			// Advance until we're at the desired row or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = '[';
			(*tmp).r = "━";

			for(int i = 0; i < 2; i++)
			{
				// Advance until we're at the desired position or we hit the end of the chain.
				while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+i+1)) ) j = (*j).next;
				tmp = new UInode; // Make a new node to insert into the list.
				(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
				(*j).next = tmp; // And point the previous node to the new node.

				(*tmp).x = newX + i + 1;
				(*tmp).y = newY;
				(*tmp).l = "━";
				(*tmp).r = "━";
			}

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+3)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX + 3;
			(*tmp).y = newY;
			(*tmp).l = "━";
			(*tmp).r = ']';
		}
	}

	// S3 - length 3
	if(ship3)
	{
		newX = ship3->getPosX();
		newY = ship3->getPosY();
		j = &start;

		if(ship3->getOrient()) // Vert.
		{
			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = "┏";
			(*tmp).r = "┓";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+1)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY + 1;
			(*tmp).l = "┃";
			(*tmp).r = "┃";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+2)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY + 2;
			(*tmp).l = "┗";
			(*tmp).r = "┛";
		}
		else
		{
			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = '[';
			(*tmp).r = "━";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+1)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX + 1;
			(*tmp).y = newY;
			(*tmp).l = "━";
			(*tmp).r = "━";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+2)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX + 2;
			(*tmp).y = newY;
			(*tmp).l = "━";
			(*tmp).r = ']';
		}
	}

	// S4 - length 3
	if(ship4)
	{
		newX = ship4->getPosX();
		newY = ship4->getPosY();
		j = &start;

		if(ship4->getOrient()) // Vert.
		{
			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = "┏";
			(*tmp).r = "┓";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+1)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY + 1;
			(*tmp).l = "┃";
			(*tmp).r = "┃";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+2)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY + 2;
			(*tmp).l = "┗";
			(*tmp).r = "┛";
		}
		else
		{
			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = '[';
			(*tmp).r = "━";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+1)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX + 1;
			(*tmp).y = newY;
			(*tmp).l = "━";
			(*tmp).r = "━";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+2)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX + 2;
			(*tmp).y = newY;
			(*tmp).l = "━";
			(*tmp).r = ']';
		}
	}

	// S5 - length 2
	if(ship5)
	{
		newX = ship5->getPosX();
		newY = ship5->getPosY();
		j = &start;

		if(ship5->getOrient()) // Vert.
		{
			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = "┏";
			(*tmp).r = "┓";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY+1)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY + 1;
			(*tmp).l = "┗";
			(*tmp).r = "┛";
		}
		else
		{
			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX;
			(*tmp).y = newY;
			(*tmp).l = '[';
			(*tmp).r = "━";

			// Advance until we're at the desired position or we hit the end of the chain.
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX+1)) ) j = (*j).next;
			tmp = new UInode; // Make a new node to insert into the list.
			(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
			(*j).next = tmp; // And point the previous node to the new node.

			(*tmp).x = newX + 1;
			(*tmp).y = newY;
			(*tmp).l = "━";
			(*tmp).r = ']';
		}
	}

	// Phase 2: insert missiles.

	gridElem *p = &mListStart; // Start at the beginning of the missile list. Last (invalid) missile will point null.
	while( (*p).next )
	{
		// Go through the grid list to find the cell to insert / overwrite the missile in.
		newX = (*p).m->getPosX();
		newY = (*p).m->getPosY();
		j = &start;
		if( ((*j).y*10)+(*j).x == ((newY)*10+(newX)) ) // Short circuit check for first node.
		{
			(*j).l = "X";
			(*j).r = "X";
		}
		else // Normal checks.
		{
			while( (*(*j).next).next && ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) < ((newY)*10+(newX)) ) j = (*j).next;
			if( ((((*((*j).next)).y) * 10) + ((*((*j).next)).x)) == ((newY)*10+(newX)) ) // Same position, missile overwrites ship segment.
			{
				(*((*j).next)).l = "X";
				(*((*j).next)).r = "X";
			}
			else // Miss, insert new node.
			{
				tmp = new UInode; // Make a new node to insert into the list.
				(*tmp).next = (*j).next; // Make the node point to the previous node's next (may be null).
				(*j).next = tmp; // And point the previous node to the new node.

				(*tmp).x = newX;
				(*tmp).y = newY;
				(*tmp).l = ".";
				(*tmp).r = '.';
			}
		}
		// And advance to the next missile.
		p = (*p).next;
	}

	// Phase 3 - Print the grid.
	j = &start;
	ClearScreen();
	// Draw top of game grid.
	std::cout << "           YOUR GRID:" << std::endl <<
	   		     "   A  B  C  D  E  F  G  H  I  L" << std::endl <<
				 "  +--+--+--+--+--+--+--+--+--+--+" << std::endl;
	int i, k;
	for(i = 0; i < 10; i++) // Row iterator.
	{
		std::cout << boost::format("%2d") % (i+1) << "|"; // Print current row index and cell preamble.
		for(k = 0; k < 10; k++) // Column iterator.
		{
			if( (*j).x == k && (*j).y == i )
			{
				std::cout << (*j).l << (*j).r << "|";
				if( (*j).next ) j = (*j).next;
			}
			else std::cout << "  |"; // Empty cell.
		}
		std::cout << std::endl << "  +--+--+--+--+--+--+--+--+--+--+" << std::endl;
	}
}
