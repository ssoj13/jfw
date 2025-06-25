#ifndef _Uv
#define _Uv

// simple point class
#include "j_framework.h"

class Uv
{
	public:
	Uv() { clear(); }
	Uv(const Uv &b) { *this = b; }
	Uv(jreal uu, jreal vv) { _u = uu; _v = vv; }
	
	void	clear			()					{ _u = 0; _v = 0; }
	Uv&		operator=		(const Uv &b)		{ if(this == &b) return(*this); _u = b._u; _v = b._v; return(*this);	}
	bool	operator==		(const Uv &b) const	{ if(_u == b._u && _v == b._v) return true; return false; }
	bool	operator!=		(const Uv &b) const	{ if(_u != b._u || _v != b._v) return true; return false; }
	jreal	operator[]		(int i) const		{ if(i==0) return _u; else return _v; }
	jreal&	operator[]		(int i)				{ if(i==0) return _u; else return _v; }

	friend std::ostream& operator<<(std::ostream& ostr, const Uv& b)
	{
		OSTR_SET
		ostr << "Uv<" << b._u << ", " << b._v << ">" << endl;
		return ostr;
	}

	private:
		jreal _u, _v;
};

#endif
