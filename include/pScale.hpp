//------------------------------------------
// pScale.hpp
// header file for the pScale class
//------------------------------------------
// Defines an abstract scale class that
// essentially governs the validity of
// certain notes in a given context.
//------------------------------------------

#ifndef __PSCALE__
#define __PSCALE__

#define UP 0
#define DOWN 1

#include <stdio.h>

#include "../include/pSystem.h"

class pScale
{
	public:
		pScale(FILE *);
		pScale(char *, short);
		void setScale(char *, short);
		short degree(short, short);
		~pScale();
		void dump(void);
		short getLength(short);

	private:
		void tokenise(char *, short *, short *);
		short degreesUp[12];
		short degreesDown[12];
		short lengthUp;
		short lengthDown;
};

#endif
