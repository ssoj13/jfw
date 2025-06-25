#ifndef _Sequence
#define _Sequence
#include "j_framework.h"

// Input: path string
// Output: vector of sequences

struct Seq
{
	jint start;
	jint end;
	jint padding;
	std::string pre;
	std::string post;
};

class Sequence
{
	public:
		Sequence() { clear(); }
		Sequence(const std::string& path) {  }
		void clear() { }
		
	private:
		std::string			_path;
		std::vector<Seq>	_seqs;

};


#endif

