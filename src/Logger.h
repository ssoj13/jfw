#ifndef _Logger
#define _Logger
#include "j_framework.h"

class Logger
{
	public:
		Logger(std::string fname)
		{
			_name = fname;
			std::string temp = Util::getTempDir() + "/" + _name;
			if(Util::subStr(_name, ".") == -1) temp += ".log";
			out.open(temp, ios::app);
			_error = out.rdstate();
			if(_error) { std::cerr << "Logger(): can't open " << temp << endl; out.close(); }
			else out << endl << endl << "--=== Logger(): " << temp << " opened for append at " << Util::getTime() << "===--" << endl;
		}

		~Logger()					{ clear(); }
		void clear()				{ out.close(); _error = 0; }

		void msg(const std::string &msg) const
		{
			if(_error) return;
			std::cout << Util::getTime() << ": " << msg << std::endl;
		}

	private:
		jint _error;
		std::string _name;
		std::ofstream out;
};

#endif