#ifndef _Part
#define _Part

// simple point class
#include "j_framework.h"

class Part
{
	public:
		Part() { clear(); }
		Part(const Part& b) { *this = b; }
		Part(const jreal& xx) { set(xx, xx, xx); }
		Part(const jreal& xx, const jreal& yy, const jreal& zz) { set(xx, yy, zz); }

		void get(jreal& xx, jreal& yy, jreal& zz) const { xx = _x; yy = _y; zz = _z; }
		void set(const jreal& xx, const jreal& yy, const jreal &zz) { _x = xx; _y = yy; _z = zz; }

		jreal x() const { return _x; }
		jreal y() const { return _y; }
		jreal z() const { return _z; }

		// vector ops
		void	clear()			{ _x = 0; _y = 0; _z = 0; }
		jreal	size() const	{ return 3; }
		jreal	length() const	{ return std::sqrt((_x*_x)+(_y*_y)+(_z*_z)); }
		
		static Part		normalize(const Part& a)			{ jreal l = a.length();  return Part(a._x/l, a._y/l, a._z/l); }
		static jreal	dot(const Part& a, const Part& b)	{ return ((a._x*b._x) + (a._y*b._y) + (a._z*b._z)); }
		static Part		cross(const Part& a, const Part& b)	{ return Part(a._y*b._z - a._z*b._y, a._z*b._x - a._x*b._z, a._x*b._y - a._y*b._x); }
		void			normalize()						{ *this = normalize(*this); }
		jreal			dot(Part& b)						{ return dot(*this, b); }
		Part				cross(Part& b)					{ return cross(*this, b); }

		// angle()
		// matrix = rotate(v1, v2)

		// Operators
		Part& operator=(const Part& b)				{ if(this == &b) { return(*this); } _x = b._x; _y = b._y; _z = b._z; return(*this); }
		bool operator==(const Part &b) const		{ if(_x == b._x && _y == b._y && _z == b._z) return true; return false; }
		bool operator!=(const Part &b) const		{ if(_x != b._x || _y != b._y || _z != b._z) return true; return false;	}
		Part operator+(const Part& b) const			{ Part r(*this); r._x += b._x; r._y += b._y; r._z += b._z; return(r); }
		Part& operator+=(const Part &b)				{ _x += b._x; _y += b._y; _z += b._z; return(*this); }
		Part operator-(const Part& b) const			{ Part r(*this); r._x -= b._x; r._y -= b._y; r._z -= b._z; return(r); }
		Part& operator-=(const Part &b)				{ _x -= b._x; _y -= b._y; _z -= b._z; return(*this); }
		Part operator/ (const jreal& div) const	{ Part r(*this); r._x /= div; r._y /= div; r._z /= div; return(r); }
		Part& operator/= (const jreal& div)		{ _x /= div; _y /= div; _z /= div; return (*this); }
		jreal operator[](int i) const			{ if(i==0) return _x; else if(i==1) return _y; else return _z; }
		jreal& operator[](int i)				{ if(i==0) return _x; else if(i==1) return _y; else return _z; }
		Part	operator* (const jreal& mult) const { Part r(*this); r._x *= mult; r._y *= mult; r._z *= mult; return(r); }
		Part& operator*= (const jreal& mult)		{ _x *= mult; _y *= mult; _z *= mult; return (*this); }
		Part& operator*= (const Mat4& m)
		{
			jreal xx, yy, zz, ww, _w = 1.0;
			xx = _x*m.get(0, 0) + _y*m.get(0, 1) + _z*m.get(0, 2) + _w*m.get(0, 3);
			yy = _x*m.get(1, 0) + _y*m.get(1, 1) + _z*m.get(1, 2) + _w*m.get(1, 3);
			zz = _x*m.get(2, 0) + _y*m.get(2, 1) + _z*m.get(2, 2) + _w*m.get(2, 3);
			ww = _x*m.get(3, 0) + _y*m.get(3, 1) + _z*m.get(3, 2) + _w*m.get(3, 3);
			if(ww != 1.0f) { xx /= ww; yy /= ww; zz /= ww; ww = 1.0f; }
			_x = xx;
			_y = yy;
			_z = zz;
			_w = ww;
			return (*this);
		}
		// glm matrices
		Part	operator* (const glm::mat3 &m) const { glm::vec3 v(_x, _y, _z); v = m*v; return Part(v.x, v.y, v.z); }
		Part&	operator*= (const glm::mat3 &m) { *this = *this * m; return (*this); }
		Part	operator* (const glm::mat4 &m) const { glm::vec4 v(_x, _y, _z, 1.0); v = m*v; return Part(v.x, v.y, v.z); }
		Part&	operator*= (const glm::mat4 &m) { *this = *this * m; return (*this); }
		// vector rotation by glm matrices
		Part rotate(const glm::mat3 &m) const { glm::vec3 v(_x, _y, _z); v = m*v; return Part(v.x, v.y, v.z); }
		Part rotate(const glm::mat4 &m) const { glm::vec4 v(_x, _y, _z, 0.0f); v = m*v; return Part(v.x, v.y, v.z); }

		// glm vectors
		Part operator+(const glm::vec3 &b) const { Part r(*this); r._x += b.x; r._y += b.y; r._z += b.z; return(r); }
		Part& operator+=(const glm::vec3 &b)	{ _x += b.x; _y += b.y; _z += b.z; return(*this); }
		Part operator+(const glm::vec4 &b) const { Part r(*this); r._x += b.x; r._y += b.y; r._z += b.z; return(r); }
		Part& operator+=(const glm::vec4 &b)	{ _x += b.x; _y += b.y; _z += b.z; return(*this); }


		friend std::ostream& operator<<(std::ostream& ostr, const Part& b)
		{
			OSTR_SET
				ostr << "Part<" << b._x << ", " << b._y << ", " << b._z << ">\n";
			return ostr;
		}

	private:
		jreal _x, _y, _z;
};


#endif
