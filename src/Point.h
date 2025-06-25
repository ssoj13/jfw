#ifndef _jpoint
#define _jpoint
#include "j_framework.h"

class Point
{
	public:
		Point()												 		{ clear(); }
		Point(const Point& p)								 		{ clear(); *this = p; }
		Point(const jint p, const jint uv=-1, const jint normal=-1)	{ clear(); _pt = p; if(uv >= 0) _uvs.push_back(uv); _normal = normal; }
	
		void clear()				{ _pt = _normal = _dummy = -1; _uvs.clear(); }
		jint get() const			{ return _pt; }
		void set(const jint& ref)	{ _pt = ref; }

		jint getUv(jint chan=0) const		{ if(uvchan(chan)) return _uvs[chan]; return -1; }
		std::vector<jint> getUvs() const	{ return _uvs; }
		void addUv(jint ref)				{ _uvs.push_back(ref); }
		void setUv(jint ref, jint chan=0)	{ resize(chan); _uvs[chan] = ref; }
		jint numuvs() const					{ return (jint)_uvs.size(); }
		bool uvchan(const jint id) const	{ if(id>=numuvs()) return false; return true; }
		void resize(jint id)				{ if(!uvchan(id)) _uvs.resize(id+1); }

		jint getN() const					{ return _normal; }
		void setN(jint nn)					{ _normal = nn; }

		void offset(jint ptOfs, jint uvOfs=0, jint nOfs=0)	{ _pt += ptOfs; for(auto &uv : _uvs) { uv += uvOfs; } _normal += nOfs; }

		// Operators
		bool	operator==(const Point &b) const	{ if(_pt == b._pt && _uvs == b._uvs) return true; return false; }
		Point&	operator=(const Point &b)			{ if(this == &b) { return(*this); } _pt = b._pt; _uvs = b._uvs; _normal = b._normal; return(*this); }
		Point&	operator=(const jint pt)			{ set(pt); return(*this); }
		jint	operator[](jint chan) const			{ if(uvchan(chan)) return _uvs[chan]; else return -1; }
		jint&	operator[](jint chan)				{ if(uvchan(chan)) return _uvs.at(chan); else return _dummy; }

		friend std::ostream& operator<<(std::ostream& ostr, const Point& b)
		{
			OSTR_SET
			int cntr = 0;
			ostr << "Point[" << b._pt << "; ";
			for(auto i : b._uvs) { if(cntr++) ostr << ", "; ostr << i; }
			ostr << "; " << b._normal << "]";
			return ostr;
		}

		// iterators compatibility
		std::vector<jint>::iterator begin() { return _uvs.begin(); }
		std::vector<jint>::iterator end() { return _uvs.end(); }
		std::vector<jint>::const_iterator begin() const { return _uvs.begin(); }
		std::vector<jint>::const_iterator end() const { return _uvs.end(); }

	private:
		jint _pt, _normal, _dummy;
		std::vector<jint> _uvs;
};

#endif
