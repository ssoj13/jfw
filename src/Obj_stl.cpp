#include "Obj_stl.h"

// TODO: fix negative indices

Obj_stl::Obj_stl(const std::string& fname)
{
	clear();
	_mesh = import(fname);
	cout << _mesh;
}

void Obj_stl::clear()
{
	_mesh.clear();
	_tokens.clear();
	_tokens.push_back("#");
	_tokens.push_back("v");
	_tokens.push_back("vt");
	_tokens.push_back("vn");
	_tokens.push_back("f");
	_error = 0;
}

Mesh Obj_stl::get() { return _mesh; }
jint Obj_stl::error() { return _error; } 

std::ostream& operator<<(std::ostream& ostr, const Obj_stl& b)
{
	OSTR_SET
	ostr << b._mesh;
	return ostr;
}

Mesh Obj_stl::import(const std::string& fname)
{
	Mesh r;
	std::vector<std::string> buf;
	std::string line;
	std::ifstream file(fname);
	jint cntr=0;
	if(!file) { return r; }
	TIMER1_START
	while(std::getline(file, line))
	{
		parseLine(line, r);
		if(cntr%100000==0)
		{
			cout << "line " << cntr << "..";
			TIMER1_END
			TIMER1_START
		}
		cntr++;
	}
	TIMER1_END
	return r;
}

void Obj_stl::parseLine(const std::string& line, Mesh& mesh)
{
	// split
	std::vector<std::string> words = Util::splitStr(line, ", ");
	auto n = words.size();
	if(!n) return;

	// #
	jint tk = 0;
	if(!words[0].compare(_tokens[tk++]))
	{
		return;
	}
	// v 0 0 0 0
	else if(!words[0].compare(_tokens[tk++]))
	{
		if(n != 4 && n != 5) return;
		jreal xx, yy, zz, ww;
		xx = Util::s2d(words[1]);
		yy = Util::s2d(words[2]);
		zz = Util::s2d(words[3]);
		if(n==5) { ww = Util::s2d(words[4]); xx /= ww; yy /= ww; zz /= ww; }
		mesh.addPt(xx, yy, zz);
		return;
	}
	// vt 0 0
	else if(!words[0].compare(_tokens[tk++]))
	{
		if(n != 3) return;
		jreal uu, vv;
		uu = Util::s2d(words[1]);
		vv = Util::s2d(words[2]);
		mesh.addUv(uu, vv);
		return;
	}
	// vn 0 0 0
	else if(!words[0].compare(_tokens[tk++]))
	{
		if(n != 4) return;
		jreal xx, yy, zz;
		xx = Util::s2d(words[1]);
		yy = Util::s2d(words[2]);
		zz = Util::s2d(words[3]);
		//mesh.addNormal(xx, yy, zz);
		return;
	}
	// f 1 4 5 7
	// f 1/2 2/12 15/31
	// f 0/0/0 1/1/1 2/2/2
	// f 1//2 2//7 3//1 2//5
	else if(!words[0].compare(_tokens[tk++]))
	{
		if(n < 4) return;
		// words loop
		Poly jPoly;
		for(auto w = 1; w<n; w++)
		{
			jint slashes = Util::countStr(words[w], "/");
			auto nums = Util::splitStr(words[w], "/");
			// indices can be negative, so we have to create flags to set pt/uv/nrm or not
			jint pt, uv, nrm, uvFlag = 0, nrmFlag = 0;
			// pt
			if(!slashes)
			{
				pt = Util::s2i(words[w]);
			}
			// pt/uv
			else if(slashes==1)
			{
				pt = Util::s2i(nums[0]);
				uv = Util::s2i(nums[1]); uvFlag = 1;
			}
			// pt//nrm OR pt/uv/nrm
			else if(slashes==2)
			{
				// 1/2/3
				if(nums.size()==3)
				{
					pt = Util::s2i(nums[0]);
					uv = Util::s2i(nums[1]); uvFlag = 1;
					nrm = Util::s2i(nums[2]); nrmFlag = 1;
				}
				else if(nums.size()==2)
				{
					pt = Util::s2i(nums[0]);
					nrm = Util::s2i(nums[1]); nrmFlag = 1;
				}
			}
			// explanation:
			// if negative index: numPts-1+negPt+1
			// if positive index: pt--, since OBJ indices are starting from 1
			if(pt<0) pt = mesh.numPts()+pt;
				else pt--;
			if(uv<0) uv = mesh.numUvs()+uv;
				else uv--;
			if(nrm<0) nrm = mesh.numNs()+nrm;
				else nrm--;
			Point jp(pt);
			if(uvFlag) jp.setUv(uv, 0);
			if(nrmFlag) jp.setN(nrm);
			jPoly.addPoint(jp);
		}
		// words loop end
		mesh.addPoly(jPoly);
	}
}
