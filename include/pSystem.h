/*------------------------------
// pSystem.h
// globals, externs, and
// constants
//------------------------------
*/

#ifndef __PSYSTEM__
#define __PSYSTEM__
#include <sys/types.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/* // Maximum number of pThreads */

#define MAXTHREADS 16

/* // Maximum number of pScales */

#define MAXSCALES 32

/* // Rests are considered to be "pitch class 12" */
/* // for the sake of storage */
/* and if notes are specified by degree instead of
   pitchClass then the pitchClass will be set to "DEG" */

#define REST 12
#define DEG 13

/* // Maximum length of input line */

#define MAXCHARS 80

/* // Maximum number of input registers */

#define IREGISTERS 256

/* // Definitions for the benefit of the harmony module */

#define NATURAL 0
#define NAT 0
#define DDIM 1
#define DIM 2
#define DIMINISHED 2
#define MIN 3
#define MINOR 3
#define PER 4
#define PERFECT 4
#define MAJ 5
#define MAJOR 5
#define AUG 6
#define AUGMENTED 6
#define DAUG 7

/* Scale definitions */

#define IONIAN 			0
#define MELODICMINOR 	1
#define HARMONICMINOR	2

/* Numerical constants */

#define TOL	0.00005

/* Globals to the pKernel */

#ifndef __DEBUG_DEF__
#ifdef __I_AM_PKERNEL__
#define __DEBUG_DEF__ 1
short debug;
#else
#define __DEBUG_DEF__ 1
extern short debug;
#endif
#endif

#endif
