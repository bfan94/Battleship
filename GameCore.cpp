/**
 * GameCore.cpp
 * Core classes for game mechanics.
 * John Greenslade, Billy Fanelli
 **/

#include "GameCore.h"

GameGrid::GameGrid(std::string name) : playerName(name) {}

FriendGrid::FriendGrid(std::string name) : GameGrid(name), ship1(nullptr), ship2(nullptr), ship3(nullptr), ship4(nullptr), ship5(nullptr) {}
EnemyGrid::EnemyGrid(std::string name) : GameGrid(name) {}

void FriendGrid::clearGrid(void)
{
	// TODO
}

void EnemyGrid::clearGrid(void)
{
	// TODO
}

int GamePiece::getPosX(void) { return this->posX; }
int GamePiece::getPosY(void) { return this->posY; }

GamePiece::GamePiece(int x, int y) : posX(x), posY(y) {}

Ship::Ship(int x, int y, bool o) : orientation(o), GamePiece(x,y) {}

bool Ship::getOrient(void) { return orientation; }

Carrier::Carrier(int x, int y, bool o) : Ship(x,y,o) {}
Battleship::Battleship(int x, int y, bool o) : Ship(x,y,o) {}
Submarine::Submarine(int x, int y, bool o) : Ship(x,y,o) {}
Destroyer::Destroyer(int x, int y, bool o) : Ship(x,y,o) {}
PatrolBoat::PatrolBoat(int x, int y, bool o) : Ship(x,y,o) {}

bool Carrier::checkValidPos(int x, int y, bool orient) // 0 = left-right
{
	if(orient) // Up-down.
	{
		if( y > 5 ) return false;
	}
	else
	{
		if( x > 5 ) return false;
	}
	if((x > 9 && x < 0)||(y > 9 && y < 0)) return false;
	return true;
}
bool Carrier::checkValidHit(int x, int y)
{
	//Look at location/orientation and determine if hit x/y is on the ship.
	if(this->getOrient()) //Up-down
	{
		if(x == this->getPosX() && y >= this->getPosY() && y <= (this->getPosY() + 5)) return true;
	}
	else
	{
		if(y == this->getPosY() && x >= this->getPosX() && y <= (this->getPosX() + 5)) return true;
	}
	return false;
}
bool Battleship::checkValidPos(int x, int y, bool orient)
{
	if(orient) // Up-down.
	{
		if( y > 6 ) return false;
	}
	else
	{
		if( x > 6 ) return false;
	}
	if((x > 9 && x < 0)||(y > 9 && y < 0)) return false;
	return true;
}
bool Battleship::checkValidHit(int x, int y)
{
	//Look at location/orientation and determine if hit x/y is on the ship.
	if(this->getOrient()) //Up-down
	{
		if(x == this->getPosX() && y >= this->getPosY() && y <= (this->getPosY() + 4)) return true;
	}
	else
	{
		if(y == this->getPosY() && x >= this->getPosX() && y <= (this->getPosX() + 4)) return true;
	}
	return false;
}
bool Submarine::checkValidPos(int x, int y, bool orient)
{
	if(orient) // Up-down.
	{
		if( y > 7 ) return false;
	}
	else
	{
		if( x > 7 ) return false;
	}
	if((x > 9 && x < 0)||(y > 9 && y < 0)) return false;
	return true;
}
bool Submarine::checkValidHit(int x, int y)
{
	//Look at location/orientation and determine if hit x/y is on the ship.
	if(this->getOrient()) //Up-down
	{
		if(x == this->getPosX() && y >= this->getPosY() && y <= (this->getPosY() + 3)) return true;
	}
	else
	{
		if(y == this->getPosY() && x >= this->getPosX() && y <= (this->getPosX() + 3)) return true;
	}
	return false;
}
bool Destroyer::checkValidPos(int x, int y, bool orient)
{
	if(orient) // Up-down.
	{
		if( y > 7 ) return false;
	}
	else
	{
		if( x > 7 ) return false;
	}
	if((x > 9 && x < 0)||(y > 9 && y < 0)) return false;
	return true;
}
bool Destroyer::checkValidHit(int x, int y)
{
	//Look at location/orientation and determine if hit x/y is on the ship.
	if(this->getOrient()) //Up-down
	{
		if(x == this->getPosX() && y >= this->getPosY() && y <= (this->getPosY() + 3)) return true;
	}
	else
	{
		if(y == this->getPosY() && x >= this->getPosX() && y <= (this->getPosX() + 3)) return true;
	}
	return false;
}
bool PatrolBoat::checkValidPos(int x, int y, bool orient)
{
	if(orient) // Up-down.
	{
		if( y > 8 ) return false;
	}
	else
	{
		if( x > 8 ) return false;
	}
	if((x > 9 && x < 0)||(y > 9 && y < 0)) return false;
	return true;
}
bool PatrolBoat::checkValidHit(int x, int y)
{
	//Look at location/orientation and determine if hit x/y is on the ship.
	if(this->getOrient()) //Up-down
	{
		if(x == this->getPosX() && y >= this->getPosY() && y <= (this->getPosY() + 2)) return true;
	}
	else
	{
		if(y == this->getPosY() && x >= this->getPosX() && y <= (this->getPosX() + 2)) return true;
	}
	return false;
}

Missile::Missile(int x, int y) : GamePiece(x,y) {}

bool Missile::getHit(void) { return isHit; }

void Missile::setHit(bool h) { isHit = h; }

void FriendGrid::placeShip(Ship *s, int n)
{
	switch (n)
	{
		case 1:
			ship1 = s;
			break;
		case 2:
			ship2 = s;
			break;
		case 3:
			ship3 = s;
			break;
		case 4:
			ship4 = s;
			break;
		case 5:
			ship5 = s;
			break;
	}
}

