//--------------------------------
// pEvent.hpp
// header file for the pEvent class
//--------------------------------

#ifndef __PEVENT__
#define __PEVENT__

// event types:

#define PEVTEMPO 		01
#define PEVCOMMENT 	02

#include <stdio.h>

extern "C" {
#include "../include/pSystem.h"
}

class pEvent
{
	public:
		pEvent(short, short);
		pEvent(short, char *);
		short type;

		pEvent *next;
		pEvent *prev;

		void add(pEvent *);
		void dump();

		short tempo;
		char comment[80];
};

#endif
