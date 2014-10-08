/**
 * net.h
 * Networking and communications routines.
 * John Greenslade, Billy Fanelli
 **/
#pragma once

void clientStart(char* addr);
void serverStart(void);
void serverEnd(void);

bool moveSend(int move);
bool moveReceive(void);

/*
#include <iostream>
#include <string>
#include <pthread.h>
#include <boost/asio.hpp>

void initServer(void);

bool send_flag;
std::string send_data;
std::string receive_data;
*/
