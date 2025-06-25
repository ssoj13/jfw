#ifndef _Bresenham
#define _Bresenham
#include "j_framework.h"

/*
http://www.mira-project.org/MIRA-doc/classmira_1_1BresenhamLineIterator.html

BresenhamLineIterator (int x0, int y0, int x1, int y1, int xInc=1, int yInc=1)
	Creates a new bresenham iterator that iterates over a line starting at coordinate (x0,y0) and ending in (x1,y1).
bool 	hasNext () const
	Returns true, if there are more points/pixels to go.
const BresenhamLineIterator & 	operator++ ()
	Advances the iterator to the next point/pixel.
const BresenhamLineIterator & 	operator-- ()
	Moves iterator backward to the previous point/pixel.
int 	x () const
	Returns current x-coordinate.
int 	y () const
	Returns current y-coordinate.
bool 	isMajorX () const
	Returns true if the line mainly moves along the x-axis.
bool 	isMajorY () const
	Returns true if the line mainly moves along the y-axis.
int 	pos () const
	Returns the current position on the line.
int 	length () const
	Returns the length of the line in points/pixels.
BresenhamLineIterator 	orthogonal ()
	Creates a new bresenham line iterator that is orthogonal to this line and starts in the current point

*/

class Bresenham
{
	public:
		Bresenham(double start, double end, uint items, uint sd=0) { set(sd); }

		void set(uint seedIn) { _seed = seedIn; std::srand(_seed); }
		uint get() { return _seed; }
		void reset() { set(1); }

		Bresenham& operator=(const Bresenham& b)
		{
			// check for self-assignment
			if(this == &b) { return(*this); }
			_seed = b._seed;
			return(*this);
		}


	private:
		uint _seed;
};

#endif
