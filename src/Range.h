#ifndef _Range
#define _Range
#include "j_framework.h"

/*
0, 1, 2, 3, 4 17 5 6 7 10-20, 11

separators: " ,";
range symbol: "-"
step symbol: ":"
all: "*"
negate: "^"
start: "s"
end: "e"

symbols are processed in order of appearance
// todo: jrange should return an iterator over all numbers - check on how to create an STL iterator

1. split the string by " ,"
2. for auto r : result
	a. * - all
	b. ^ - remove
	c. - - range
	d. 

2. if len()==1 && * - include all
3. if len()==1 && ^ - exclude all
4. if contains "-" - call range(a, b)
5. if contains "-:" - call range(a, b, c)
6. "s" means "start", 0
7. "e" means "end", "Nth" element

check on how to make an iterator from indices


1. int
2. string - full
3. string - compressed

*/

class Range
{
	public:
	//TODO: test with range of 1-2 elements
	//TODO: add range logical ops: and, or, not, xor

	// constructors
	Range() { clear(); }
	Range(const std::string& r) { set(r); }
	Range(const std::vector<jint>& r) { set(r); }
	Range(const jint s=0, const jint e=0, const jint step=1) { set(s, e, step); }

	// set
	void set(const std::string& r) { clear(); _range = str2int(r, 0); }
	void set(const std::vector<jint>& r) { clear(); _range = r; }
	void set(const jint s=0, const jint e=0, const jint step=1) { clear(); add(s, e, step); }

	//add
	void add(const std::string& r) { Util::joinVec(_range, str2int(r, 0)); }
	void add(const std::vector<jint>& r) { Util::joinVec(_range, r); }
	void add(jint s=0, jint e=0, jint step=1) { if(s > e) std::swap(s, e); for(auto i = s; i<=e; i += step) { _range.push_back(i); } }	// sanity checks here

	std::vector<jint> asInt() const { return _range; }
	std::string asStr() const { return int2str(_range); }
	std::string asFullStr() const { return int2fullStr(_range); }

	// misc
	void sort() { Util::sortVec(_range); }
	void uniq() { Util::uniqVec(_range); }
	size_t size() { return _range.size(); }
	void clear() { _range.clear(); }


	/**
	Converts 0 1 2 3 4 5 17 18 19 21 -> 0-5 17-19 21
	with optional sorting
	*/
	std::string int2str(const std::vector<jint> &range, int sort=0) const
	{
		std::string r, tstr;
		jint s, e, sz, cur, next;

		// sanity checks
		if(range.size() == 0) return r;
		if(range.size() == 1) return std::string(Util::i2s(range[0]));

		std::vector<jint> rng(range);		// copy input array for sorting
		if(sort) { Util::sortUniqVec(rng); }	// both sorting/making unique sorting if on
		sz = (jint)rng.size();
		s = e = rng[0];

		// loop
		for(auto i = 0; i < sz; i++)
		{
			//if(i == sz-2) __debugbreak();
			cur = rng[i];
			if(i<sz-1) next = rng[i+1]; else next = cur-1;	// if index is out of vector - just use value that 100% != cur
			if(cur+1 == next) { e = next; continue; }		// range: set new end

															// Ok, cur != next here: dump the numbers s..cur
			{
				// start == end: single number
				if(s == e) { tstr = Util::i2s(s); }
				// start != end: range
				else
				{
					if(e-s == 1) { tstr = Util::i2s(s) + " " + Util::i2s(e); }	// range of two nearbys: 10 11
					else { tstr = Util::i2s(s) + "-" + Util::i2s(e); }			// bigger range: 10-20
				}
			}
			if(r.size()) { r += " "; }					// add space in front if not at start
			r += tstr;
			s = e = next;
		}
		return r;
	}

	/**
	Converts range string to vector<int>
	*/
	std::vector<jint> str2int(const std::string& instr, int sort=0) const
	{
		std::vector<jint> ri, ti;
		std::vector<std::string> tt;

		tt = Util::splitStr(instr, " ,");
		for(auto rng : tt)
		{
			ti = unroll(rng);
			Util::joinVec(ri, ti);
		}
		if(sort) { Util::uniqVec(ri); }	// sorting if on
		return ri;
	}

	/**
	Actual handler for range segments: unrolls std::string("0-10:2") into vector<int>
	possible cases:
	10
	1-10
	1-10:2
	*/
	std::vector<jint> unroll(const std::string &instr) const
	{
		std::vector<jint> ri;
		int step = 1;

		auto range = Util::splitStr(instr, "-:");
		if(!range.size()) { return ri; }
		else if(range.size() == 1)
		{
			ri.push_back(Util::s2i(range[0]));
		}
		else if(range.size() > 1)
		{
			auto s = Util::s2i(range[0]);
			auto e = Util::s2i(range[1]);
			if(range.size() > 2) { step = Util::s2i(range[2]); }	// step
			for(auto i = s; i<=e; i += step) { ri.push_back(i); }
		}
		return ri;
	}

	
	/**
	Converts vector of ints to simple string. One int = one number, no automation
	*/
	std::string int2fullStr(const std::vector<jint> &range, int sort=0) const
	{
		std::string r;
		for(auto i : range) { if(r.size()) r += " "; r += Util::i2s(i); }
		return r;
	}


	friend std::ostream& operator<<(std::ostream& ostr, const Range& b)
	{
		OSTR_SET
			ostr << b.asStr() << endl;
		return ostr;
	}



	/*jrange& joperator=(const jrange &b)
	{
	if(this == &b) { return(*this); }	// check for self-assignment
	_u = b._u;
	_v = b._v;
	return(*this);
	}

	bool joperator==(const jrange &b) const
	{
	if(_u == b._u && _v == b._v) return true;
	return false;
	}

	bool joperator!=(const jrange &b) const
	{
	if(_u != b._u || _v != b._v) return true;
	return false;
	}

	jnum joperator[](int i) const
	{
	if(i==0) return _u;
	else return _v;
	}

	// returns ref to which new value can be assigned
	jnum& joperator[](int i)
	{
	if(i==0) return _u;
	else return _v;
	}

	void jclear()
	{
	_u = 0;
	_v = 0;
	}

	ostream& operator<<(std::ostream& ostr, const jrange& b)
	{
	OSTR_SET
	ostr << "jrange<" << b._u << ", " << b._v << ">\n";
	return ostr;
	}
	*/

		std::vector<jint>::iterator			begin()			{ return _range.begin(); }
		std::vector<jint>::iterator			end()			{ return _range.end(); }
		std::vector<jint>::const_iterator	begin() const	{ return _range.begin(); }
		std::vector<jint>::const_iterator	end() const		{ return _range.end(); }
		
	private:		
		std::vector<jint> _range;
};

#endif
