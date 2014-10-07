/**
 * TUI.h
 * Main graphics and interface routines.
 * John Greenslade, Billy Fanelli
 **/
#pragma once

#include <unistd.h>
#include <term.h>
#include <boost/format.hpp>
#include <string>
#include "GameCore.h"

// Wipe terminal clean so that drawings can be pretty.
void ClearScreen(void);

struct UInode
{
	int x,y;
	std::string l, r;
	UInode *next = nullptr;
};
