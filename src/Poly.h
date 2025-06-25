#ifndef _Poly
#define _Poly
#include "j_framework.h"

class Poly
{
	public:
		Poly() { clear(); }
		Poly(const Poly& b) { *this = b; }

		void	clear() { _points.clear(); }
		jint	numpts() const { return (jint)_points.size(); }
		jint	numuvs() const { return (jint)_points[0].numuvs(); }

		void	set(const std::vector<Point>& b) { _points = b; }
		void	addPoint(const Point& p) { _points.push_back(p); }
		void	addPoint(const jint p, const jint uv=-1) { if(uv != -1) _points.push_back(Point(p, uv)); else _points.push_back(Point(p)); }
		void	offsetPoints(jint ofs, jint uvs) { for(auto &p : _points) p.offset(ofs, uvs); }

		// Operators
		bool	operator==(const Poly &b) const { if(_points == b._points) return true; return false; }
		bool	operator!=(const Poly &b) const { if(_points != b._points) return true; return false; }
		Poly&	operator=(const Poly &b)
		{
			if(this == &b) return(*this);
			_points = b._points;
			return(*this);
		}
		Poly&	operator=(const std::vector<Point> &b) { _points = b; return(*this); }
		// these 2 _add points_
		Poly&	operator+=(const Point &b) { addPoint(b); return(*this); }
		Poly&	operator+=(const jint b) { addPoint(Point(b)); return(*this); }
		Point	operator[](const jint i) const { return _points[i]; }
		Point&	operator[](const jint i) { return _points[i]; }		// returns ref to which new value can be assigned

		friend std::ostream& operator<<(std::ostream& ostr, const Poly& b)
		{
			OSTR_SET
				jint cntr = 0;
			ostr << "Poly(" << b.numpts() << ")<";
			for(auto &i : b._points) { if(cntr++) ostr << ", "; ostr << i; }
			ostr << ">" << endl;
			return ostr;
		}

		std::vector<Point>::iterator begin() { return _points.begin(); }
		std::vector<Point>::iterator end() { return _points.end(); }
		std::vector<Point>::const_iterator begin() const { return _points.begin(); }
		std::vector<Point>::const_iterator end() const { return _points.end(); }

	private:
		std::vector<Point> _points;
		// and per-poly vars here
};

#endif
