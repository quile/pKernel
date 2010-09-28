/* --------------------------------------------------------------------
 * pKernel - System for real-time musical transformation
 * (C) kd 2010
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


//------------------------------------------
// pHarm.hpp
// header file for the pHarm class
// that defines the harmonic
// structure of the music
//------------------------------------------
// Each harmony is described in
// terms of its duration, its
// root, its interval (maximum interval
// above the root, like seventh, thirteenth,
// etc.), its octave (of its lowest note)
// and its inversion (represented by a
// decimal: 0 for root position, 1 for
// first inversion, etc. etc.)
//------------------------------------------

#ifndef __PHARM__
#define __PHARM__

#include <stdio.h>
#include "pNote.hpp"
#include "pScale.hpp"

class pHarm
{
	public:
		pHarm(void);
		pHarm(FILE *);
		pHarm (char *, short, short, short);
		~pHarm (void);
		void add(pHarm *);
		void setScale(void *, short);
		pHarm *nextHarm;
		pHarm *prevHarm;
		pScale *scale;
		void dump(short);
		void setDuration(char *);
		void setDurationF(float);
		char *getDurationC(void);
		float getDurationF(void);
		void getChordInfo (char *, short *, short *, short *);
		pNote *getChordNotes(pNote *, pScale *);
									// returns a list of notes in the
									// chord
		short getInversion(void);
		void	clone(pHarm *);	        // like a copy contructor
		void	setRoot(short);	        // sets the root note
		void	setInversion(short);	// sets the inversion
		void	setOctave(short);		// sets the octave
		void	setAlteration(short);	// sets the alteration of the root note
		short	getRoot(void);		    // returns the root PC
		short getOctave(void);          // returns the octave
		short getAlteration(void);	    // returns the alteration
		float fdur;
		short size;
		short alteration;		// number of semitones that the root is moved
								// so that one can say "-II" to mean "flat 2"
		short chordTones[12];
		short virt;			    // virtual harmonies can be changed from
							    // from key to key, mode to mode, scale to scale.

	private:
		short root;
		short inversion;
		short octave;
		char duration[12];
		short quality;			// major or minor (this is important in determining
								// the *third* within any scale context)
		//pNote chordTones[12];
		pHarm *circle;			// for wont of a better term, this is used
								// for altered chords.

		short alter(short, short, short);	// alters intervals
};

#endif
