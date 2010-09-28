//--------------------------------------
// pAddress : used by pCompiler only
// on the first pass to store locations
// of labels that are to be resolved on
// the second pass
//--------------------------------------

#ifndef __PADDRESS__
#define __PADDRESS__

#include <stdio.h>
#include <string.h>

extern "C" {
#include "../include/pSystem.h"
}

class pAddress
{
	public:
		pAddress (char *lab, long off);
		~pAddress (void);
		void add(pAddress *p);
		char label[16];
		long offset;
		pAddress *find(char *);
		void dump(short);

		pAddress *nextAddress;
};

#endif
