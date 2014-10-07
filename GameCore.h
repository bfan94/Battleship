/**
 * GameCore.h
 * Core classes for game mechanics.
 * John Greenslade, Billy Fanelli
 **/
#pragma once

#include <string>

/*
   GamePiece - Abstract container for a game piece - missile or ship
*/
class GamePiece
{
private:
	int posX, posY;
public:
	int getPosX(void);
	int getPosY(void);
	GamePiece(int x, int y); // Position is set during construction.
};

/*
   Missile
*/
class Missile : public GamePiece
{
private:
	bool isHit;
public:
	bool getHit(void);
	void setHit(bool h);
	Missile(int x, int y);
};

/*
   Ship
*/
class Ship : public GamePiece
{
private:
	bool orientation; // 0 = left-right, 1 = up-down
public:
	bool getOrient(void);
	virtual bool checkValidPos(int x, int y, bool orient) = 0; // Needs to be specifically implemeted by each ship type.
	virtual bool checkValidHit(int x, int y) = 0; // Same.
	Ship(int x, int y, bool o);
};

/*
   Carrier - ship type 1
*/
class Carrier : public Ship
{
public:
	bool checkValidPos(int x, int y, bool orient);
	bool checkValidHit(int x, int y);
	Carrier(int x, int y, bool o);
};
/*
   Battleship - ship type 2
*/
class Battleship : public Ship
{
public:
	bool checkValidPos(int x, int y, bool orient);
	bool checkValidHit(int x, int y);
	Battleship(int x, int y, bool o);
};
/*
   Submarine - ship type 3
*/
class Submarine : public Ship
{
public:
	bool checkValidPos(int x, int y, bool orient);
	bool checkValidHit(int x, int y);
	Submarine(int x, int y, bool o);
};
/*
   Destroyer - ship type 4
*/
class Destroyer : public Ship
{
public:
	bool checkValidPos(int x, int y, bool orient);
	bool checkValidHit(int x, int y);
	Destroyer(int x, int y, bool o);
};
/*
   PatrolBoat - ship type 5
*/
class PatrolBoat : public Ship
{
public:
	bool checkValidPos(int x, int y, bool orient);
	bool checkValidHit(int x, int y);
	PatrolBoat(int x, int y, bool o);
};

/*
   Grid element linked list struct - used to store missile hit sequences in a grid.
*/
struct gridElem 
{
	Missile *m;
	gridElem *next = nullptr; // If the pointer is NULL, the element is the end sentinel of the list. Its missile is not valid.
};

/*
   GameGrid - Abstract class that represents an entire game grid, friend or enemy.
*/
class GameGrid
{
private:
	std::string playerName;
public:
	gridElem mListStart; // Must be public to allow main logic to manipulate internal values.
	virtual void print(void) = 0; // Must be implemented by FriendGrid or EnemyGrid
	virtual void clearGrid(void) = 0; // Same.
	GameGrid(std::string name); // Constructor with name passed in.
};

/*
   FriendGrid - The player's home grid
*/
class FriendGrid : public GameGrid
{
private:
	Ship *ship1;
	Ship *ship2;
	Ship *ship3;
	Ship *ship4;
	Ship *ship5;
public:
	int receiveHit(Missile m); // Insert missile data into grid.
	void placeShip(Ship *s, int n); // Insert ship into grid. No error checking!
	void print(void); // Print out grid.
	void clearGrid(void); // Empty all missiles and ships.
	FriendGrid(std::string name);
};

/*
   EnemyGrid
*/
class EnemyGrid : public GameGrid
{
public:
	int setHit(Missile m); // Insert missile hit/miss data into grid.
	void print(void); // Print out grid.
	void clearGrid(void); // Empty all missiles and ships.
	EnemyGrid(std::string name);
};

// Global grids vars.
extern FriendGrid *fg;
extern EnemyGrid *eg;

extern Ship* s1;
extern Ship* s2;
extern Ship* s3;
extern Ship* s4;
extern Ship* s5;

extern bool gameOver;

extern int hitCount;
