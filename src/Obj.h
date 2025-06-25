#ifndef _Obj
#define _Obj
#include "j_framework.h"

/*
1. avoid redundant scanning
2. each parser should parse the string to the end and return bool if it was succesful or not
parseObj()
parse_vtn()
parse_f()
readInt()
readDouble()
chomp()
cMatch()
cSearch()
*/

class Obj
{
	public:
		Obj(const std::string& fname) { if(!load(fname)) clear(); }

		bool load(const std::string& fname)
		{
			clear();
			std::string buf;
			if(!readFile(fname, buf)) return false;	// not overriding _error that came from deeper methods
			parseObj(buf, _mesh);
			_error = 0;
			return true;
		}
		jint error() const { return _error; }
		Mesh get() const { return _mesh; }

		friend std::ostream& operator<<(std::ostream& ostr, const Obj& b)
		{
			OSTR_SET
			ostr << b._mesh;
			return ostr;
		}


		// private
		void clear()
		{
			_mesh.clear();
			_error = 0;
			_whitespace = std::string(" \r");
			_cr = std::string("\n");
			_numbers = std::string("-.0123456789e");
			_dot = std::string(".");
			_minus = std::string("-");
			_plus = std::string("+");
			_slash = std::string("/");
			_e = std::string("e");
			_hash = std::string("#");
			_lines = 0;
		}


		// internal func
		bool readFile(const std::string& fname, std::string& buf)
		{
			Mesh r;
			TIMER1_START
			std::ifstream infile(fname);
			if(!infile.good()) { _error = -1;  return false; }
			infile.seekg(0, std::ios::end);
			size_t flen = infile.tellg();
			buf.clear();
			buf.resize(flen, ' ');
			infile.seekg(0);
			infile.read(&buf[0], flen);
			infile.close();
			TIMER1_END
			return true;
		}

		// ---==== PARSER ====---
		void parseObj(const std::string& buf, Mesh& mesh)
		{
			TIMER2_START
			mesh.clear();
			jint cntr = 0, linesCntr = 0, kiloTokensDone = 1;
			size_t sz = buf.size();
			while(cntr<sz)
			{
				// parsers should return EOL
				if(parse_vtn(buf, cntr, mesh)) goto l_EOL;
				if(parse_f(buf, cntr, mesh)) goto l_EOL;
				if(parse_comment(buf, cntr, mesh)) goto l_EOL;

			l_EOL:
				cSearch(buf, cntr, _cr);
				cntr++;
				linesCntr++;
				if(linesCntr>=1000)
				{
					cout << "parseObj(): " << kiloTokensDone++ << "k lines parsed.." << endl;
					linesCntr = 0;
				}
			}
			TIMER2_END
		}

		// on comment line moves index to the ending EOL
		bool parse_comment(const std::string& buf, jint& cntr, Mesh& mesh) const
		{
			if(!cMatch(buf, cntr, "#")) return false;
			cSearch(buf, cntr, _cr);
			return true;
		}

		// parser always should return the index of EOL so parseObj() could count lines
		bool parse_vtn(const std::string& buf, jint& cntr, Mesh& mesh) const
		{
			jreal xx, yy, zz, uu, vv, ww;

			if(!cMatch(buf, cntr, "v")) return false;
			cntr++;
			if(cMatch(buf, cntr, "t"))
			{
				cntr++;
				readDouble(buf, cntr, uu);
				readDouble(buf, cntr, vv);
				mesh.addUv(uu, vv);
			}
			else if(cMatch(buf, cntr, "n"))
			{
				cntr++;
				readDouble(buf, cntr, xx);
				readDouble(buf, cntr, yy);
				readDouble(buf, cntr, zz);
				mesh.addN(xx, yy, zz);
			}
			else
			{
				readDouble(buf, cntr, xx);
				readDouble(buf, cntr, yy);
				readDouble(buf, cntr, zz);
				if(readDouble(buf, cntr, ww)) { xx /= ww; yy /= ww; zz /= ww; }
				mesh.addPt(xx, yy, zz);
			}
			cSearch(buf, cntr, _cr);
			return true;
		}

		// 1
		// 1/2
		// 0/0/0
		// 1//2
		bool parse_f(const std::string& buf, jint& cntr, Mesh& mesh) const
		{
			Poly jPoly;
			jint pt, uv = -1, nrm = -1;
			bool f_uv, f_n;
			size_t sz = buf.size();

			if(!cMatch(buf, cntr, "f")) return false;
			cntr++;

			while(!cMatch(buf, cntr, _cr) && cntr < sz)
			{
				f_uv = f_n = false;
				// pt
				readInt(buf, cntr, pt);
				if(!cMatch(buf, cntr, _slash)) goto l_process;	// just a point, process
				cntr++;
				// uv
				if(readInt(buf, cntr, uv)) f_uv = 1;				// do we have uv after slash?
				if(!cMatch(buf, cntr, _slash)) goto l_process;	// just a point, process
				cntr++;
				// normal
				if(readInt(buf, cntr, nrm)) f_n = 1;

			l_process:
				// indices are starting from 1 in OBJ
				// if N is negative - getting size()-N
				if(pt<0) pt = mesh.numPts()+pt; else pt--;
				Point jp(pt);

				if(f_uv) { if(uv<0) uv = mesh.numUvs()+uv; else uv--; jp.setUv(uv, 0); }
				if(f_n) { if(nrm<0) nrm = mesh.numNs()+nrm; else nrm--;	jp.setN(nrm); }
				jPoly.addPoint(jp);
				chomp(buf, cntr);	// for the case there's whitespace-EOL after the last token - sponza.obj
			}
			mesh.addPoly(jPoly);
			return true;
		}













		// gives next symbol after whitespace
		void chomp(const std::string& buf, jint& cntr) const
		{
			size_t sz = buf.size();
			while(cMatch(buf, cntr, _whitespace) && cntr < sz) cntr++;
		}


		// Reads int at Buf[cntr], moves cntr to the next non-numeric char
		bool readInt(const std::string& buf, jint& cntr, jint& num) const
		{
			bool f_success = false, f_dot = false, f_minus = false;
			num = 0;

			chomp(buf, cntr);							// eat whitespace
			if(cMatch(buf, cntr, _cr)) return false;	// fail if there's EOL after whitespace
			while(isNumber(buf, cntr))
			{
				f_success = true;
				if(cMatch(buf, cntr, _minus)) { f_minus = true; cntr++; continue; }
				jint digit = buf[cntr]-48;
				if(digit>=0 && digit<=9) num = num*10 + digit;
				cntr++;
			}
			// if no number found after whitespace - return fail
			if(!f_success) { return false; }
			if(f_minus) num = -num;
			return true;
		}


		// from given string[offset] makes double number of returns false
		bool readDouble(const std::string& buf, jint& cntr, jreal& num) const
		{
			jint digit, left = 0, right = 0, rdiv = 1, exponent = 0;
			bool f_success = false, f_dot = false, f_minus = false, f_exp = false, f_expMinus = false;

			chomp(buf, cntr);
			if(cMatch(buf, cntr, _cr)) return false;	// fail if there's EOL after whitespace
														// reading
			while(isNumber(buf, cntr))
			{
				f_success = true;
				// dots, minuses, exponents, etc
				if(cMatch(buf, cntr, _dot)) { f_dot = true;		cntr++; continue; }
				if(cMatch(buf, cntr, _minus)) { f_minus = true;	cntr++; continue; }
				if(cMatch(buf, cntr, _e))
				{
					f_exp = true;
					cntr++;
					if(cMatch(buf, cntr, _minus)) f_expMinus = true;
					cntr++;
					continue;
				}
				// digits
				digit = buf[cntr]-48;
				if(digit>=0 && digit<=9)
				{
					if(f_exp) exponent = exponent*10 + digit;
					else
					{
						if(f_dot) { right = right*10 + digit; rdiv *= 10; }
						else { left = left*10 + digit; }
					}
				}
				cntr++;
			}
			// if no number found after whitespace - return fail
			if(!f_success) { return false; }

			// post-process
			num = (double)left + ((double)right/(double)rdiv);
			if(f_minus) num = -num;
			if(f_exp)
			{
				exponent = std::pow(10, exponent);
				if(f_expMinus) num /= exponent; else num *= exponent;
			}
			return true;
		}








		// returns the position of next found symbol from 2nd input
		bool cSearch(const std::string& buf, jint& cntr, const std::string& b) const
		{
			size_t sz = buf.size();
			while(cntr < sz)
			{
				for(auto &c : b) if(buf[cntr] == c) return true;
				cntr++;
			}
			return false;
		}

		// checks string[symbol] against multple symbols
		bool cMatch(const std::string& buf, const jint& cntr, const std::string& b) const
		{
			for(auto &c : b) if(buf[cntr] == c) return true;
			return false;
		}


		// checks full match of string[offset] with string
		bool sMatch(const std::string& buf, const jint& cntr, const std::string& b) const
		{
			size_t sz = buf.size();
			if(b.size() < sz) sz = b.size();
			for(auto i = 0; i<sz; i++) { if(buf[i+cntr] != b[i]) return false; }
			return true;
		}

		bool isNumber(const std::string& str, const jint& cntr) const
		{
			if(cMatch(str, cntr, _numbers)) return true; else return false;
		}


	private:
		Mesh _mesh;
		jint _error, _lines;
		std::string _whitespace, _cr, _numbers, _dot, _minus, _plus, _slash, _hash, _e;
};
#endif

