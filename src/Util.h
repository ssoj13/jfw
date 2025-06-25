// helper class to store all print/service funcs in a convenient way
// TODO: convert all for() loops to refs
// TODO: make as many const funcs as possible
#ifndef _Util
#define _Util


class Util
{
	public:
		// --============== strings =============--
		static std::vector<std::string> splitStr(const std::string& in, const std::string& splitters)
		{
			std::string t1, t2;
			std::vector<std::string> r;
			size_t l = in.length();
			// input string
			for(auto i = 0; i < l; i++)
			{
				t1 = in[i];
				if(Util::inStr(t1, splitters))
				{
					if(t2.length() > 0)
					{
						r.push_back(t2);
						t2.clear();
						continue;
					}
				}
				else {t2.push_back(t1[0]);}
			}
			if(t2.length() > 0) r.push_back(t2);
			return r;
		}

		// removes all the given symbols from string
		static std::string filterStr(const std::string &instr, const std::string &chars)
		{
			std::string r, t1;
			for(auto i = 0; i < instr.length(); i++)
			{
				t1 = instr[i];
				if(Util::inStr(t1, chars)) continue;
				else { r.push_back(t1[0]); }
			}
			return r;
		}

		// bool - checks string for _any_ symbol in splitters
		static bool inStr(const std::string& in, const std::string& splitters)
		{
			for(auto i : in)
			{
				for(auto s : splitters)
				{
					if(i == s) return true;
				}
			}
			return false;
		}

		// searches for substring, returns either offset or -1
		static jint subStr(const std::string& in, const std::string& sub)
		{
			size_t pos = in.find(sub);
			if(pos != std::string::npos) return (jint)pos;
			return -1;
		}

		// counts number of times splitters encountered
		static jint countStr(const std::string& in, const std::string& splitters)
		{
			jint cntr = 0;
			for(auto i : in)
			{
				for(auto s : splitters)
				{
					if(i == s) cntr++;
				}
			}
			return cntr;
		}

		// --============== vectors =============--
		template <class T> static void sortVec(std::vector<T>& vec)
		{
			if(vec.size() < 2) return;
			std::sort(vec.begin(), vec.end());
		}

		template <class T> static void uniqVec(std::vector<T>& vec)
		{
			//http://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
			// these 2 are break the elem order:
			// fastest
			//std::sort(vec.begin(), vec.end());
			//vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
			// slowest
			//std::set<T> st(vec.begin(), vec.end());
			//vec.assign(st.begin(), st.end());

			std::vector<T> r;
			for(auto e : vec) { if(isinVec(r,e)!=-1) r.push_back(e); }
			vec = r;
		}

		template <class T> static void sortUniqVec(std::vector<T>& vec)
		{
			// fastest
			std::sort(vec.begin(), vec.end());
			vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
		}

		template <class T> static void joinVec(std::vector<T>& v1, const std::vector<T>& v2)
		{
			v1.insert(v1.end(), v2.begin(), v2.end());
		}

		template <class T> static jint isinVec(const std::vector<T>& vec, const T& e)
		{
			auto pos = std::find(vec.begin(), vec.end(), e);
			if(pos != vec.end()) return std::distance(vec.begin(), pos);
			return -1;
		}

		// --============== hashes/rnd =============--
		// http://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
		static inline uint hash_xor(uint x)
		{
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x);
			return x;
		}

		// check Doom3 sources
		// https://en.wikipedia.org/wiki/MurmurHash
		// uint h = hash_murmur3_32((char*)&i, 4, 1);
		static inline uint32_t hash_murmur3_32(const char *key, uint32_t len, uint32_t seed)
		{
			static const uint32_t c1 = 0xcc9e2d51;
			static const uint32_t c2 = 0x1b873593;
			static const uint32_t r1 = 15;
			static const uint32_t r2 = 13;
			static const uint32_t m = 5;
			static const uint32_t n = 0xe6546b64;

			uint32_t hash = seed;

			const int nblocks = len / 4;
			const uint32_t *blocks = (const uint32_t *)key;
			int i;
			for(i = 0; i < nblocks; i++) {
				uint32_t k = blocks[i];
				k *= c1;
				k = (k << r1) | (k >> (32 - r1));
				k *= c2;

				hash ^= k;
				hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
			}

			const uint8_t *tail = (const uint8_t *)(key + nblocks * 4);
			uint32_t k1 = 0;

			switch(len & 3)
			{
				case 3:
					k1 ^= tail[2] << 16;
				case 2:
					k1 ^= tail[1] << 8;
				case 1:
					k1 ^= tail[0];

					k1 *= c1;
					k1 = (k1 << r1) | (k1 >> (32 - r1));
					k1 *= c2;
					hash ^= k1;
			}
			hash ^= len;
			hash ^= (hash >> 16);
			hash *= 0x85ebca6b;
			hash ^= (hash >> 13);
			hash *= 0xc2b2ae35;
			hash ^= (hash >> 16);
			return hash;
		}

		static double rndf(double mn, double mx)
		{
			static thread_local std::default_random_engine g;
			std::uniform_real_distribution<double> distreal(mn, mx);
			return distreal(g);
		}

		static int rndi(int mn, int mx)
		{
			//static thread_local std::mt19937 g;
			static thread_local std::default_random_engine g;
			std::uniform_int_distribution<int> dist(mn, mx);
			return dist(g);
		}

		// --============== maya utilities =============--
		// BBoxes
		static void resizeBB(MBoundingBox &bb, double v)
		{
			//MPoint mn = bb.center+((bb.min()-bb.center())*v);
			//MPoint mx = bb.center+((bb.max()-bb.center())*v);
			MPoint mn = bb.center() + ((bb.min()-bb.center())*v);
			MPoint mx = bb.center() + ((bb.max()-bb.center())*v);
			bb = MBoundingBox(mn, mx);
		}

		// Matrices
		// Order is: MMatrix(row, col), i.e. reading in ---> hor. lines --->
		static std::vector<double> getMatrixScale(const MMatrix &mm) {
			std::vector<double> r;
			for(auto i = 0; i<3; i++)
			{
				MVector v = MVector(mm(i, 0), mm(i, 1), mm(i, 2));
				r.push_back(v.length());
			}
			return r;
		}

		static MMatrix getTMatrix(const MPoint &pt) {
			MMatrix mm;
			mm(3, 0) = pt.x;
			mm(3, 1) = pt.y;
			mm(3, 2) = pt.z;
			return mm;
		}

		static MMatrix getTRMatrix(const MPoint &pt, const MVector &x, const MVector &y, const MVector &z) {
			MMatrix mm;
			mm(0, 0) = x.x;
			mm(0, 1) = x.y;
			mm(0, 2) = x.z;
			//
			mm(1, 0) = y.x;
			mm(1, 1) = y.y;
			mm(1, 2) = y.z;
			//
			mm(2, 0) = z.x;
			mm(2, 1) = z.y;
			mm(2, 2) = z.z;
			// pt
			mm(3, 0) = pt.x;
			mm(3, 1) = pt.y;
			mm(3, 2) = pt.z;
			return mm;
		}

		static MMatrix getTSMatrix(const MPoint &pt, float sx, float sy, float sz)
		{
			MMatrix mm;
			MVector vx, vy, vz;
			vx = MVector(1,0,0)*sx;
			vy = MVector(0,1,0)*sy;
			vz = MVector(0,0,1)*sz;

			mm(0, 0) = vx.x;
			mm(0, 1) = vx.y;
			mm(0, 2) = vx.z;
			//
			mm(1, 0) = vy.x;
			mm(1, 1) = vy.y;
			mm(1, 2) = vy.z;
			//
			mm(2, 0) = vz.x;
			mm(2, 1) = vz.y;
			mm(2, 2) = vz.z;
			// pt
			mm(3, 0) = pt.x;
			mm(3, 1) = pt.y;
			mm(3, 2) = pt.z;
			return(mm);
		}


		static MMatrix getTRSMatrix(const MPoint &pt, const MVector &x, const MVector &y, const MVector &z, float sx, float sy, float sz) {
			MMatrix mm;
			MVector vx, vy, vz;
			vx = x*sx;
			vy = y*sy;
			vz = z*sz;

			mm(0, 0) = vx.x;
			mm(0, 1) = vx.y;
			mm(0, 2) = vx.z;
			//
			mm(1, 0) = vy.x;
			mm(1, 1) = vy.y;
			mm(1, 2) = vy.z;
			//
			mm(2, 0) = vz.x;
			mm(2, 1) = vz.y;
			mm(2, 2) = vz.z;
			// pt
			mm(3, 0) = pt.x;
			mm(3, 1) = pt.y;
			mm(3, 2) = pt.z;
			return(mm);
		}

		MMatrix getSMatrix(float sx, float sy, float sz)
		{
			MMatrix mm;
			mm(0, 0) = sx;
			mm(1, 1) = sy;
			mm(2, 2) = sz;
			return mm;
		}



		// print everything to cout
		static void print(const MPoint &pt, const std::string &msg) {
			std::cout << msg << ": " << pt.x << ", " << pt.y << ", " << pt.z << ", " << pt.w << std::endl;
		}
		// print everything to cout
		static void print(const MVector &v, const std::string &msg)
		{
			std::cout << msg << ": " << v.x << ", " << v.y << ", " << v.z << std::endl;
		}
		static void print(const MMatrix &mm, const std::string &msg) {
			cout << msg;
			for(auto i = 0; i<4; i++) {
				for(auto j = 0; j<4; j++) {
					cout << mm(i, j) << " ";
				}
			}
			cout << endl;
		}
		static void print(const MIntArray &arr, const std::string &msg) {
			cout << arr.length() << ", " << msg << endl;
			for(uint i = 0; i<arr.length(); i++) cout << arr[i] << ", ";
			cout << endl;
		}
		static void print(const MDoubleArray &arr, const std::string &msg) {
			cout << arr.length() << ", " << msg << endl;
			for(uint i = 0; i<arr.length(); i++) cout << arr[i] << ", ";
			cout << endl;
		}
		static void print(const MPointArray &arr, const std::string &msg) {
			cout << arr.length() << ", " << msg << endl;
			for(uint i = 0; i<arr.length(); i++) print(arr[i], "pt: ");
			cout << endl;
		}



		// locators
		static MObject locPt(const MPoint &pt)
		{
			MDagModifier dagMod;
			MObject moLoc = dagMod.createNode("locator");
			dagMod.doIt();
			MFnTransform trFn(moLoc);
			trFn.setTranslation(MVector(pt), MSpace::kWorld);
			return moLoc;
		}

		static MObject locMM(const MMatrix &mm)
		{
			MDagModifier dagMod;
			MObject moLoc = dagMod.createNode("locator");
			dagMod.doIt();
			MTransformationMatrix trm(mm);
			MFnTransform trFn(moLoc);
			trFn.set(trm);
			return moLoc;
		}

		// --============== fit/clamp/degrees/etc ================--
		static double fit(double v, double oMin, double oMax, double nMin, double nMax)
		{
			//division by zero protection
			if(oMin==oMax || nMin==nMax) return nMin;
			if(oMin>oMax) std::swap(oMin, oMax);
			return ((v-oMin)/(oMax-oMin)*(nMax-nMin)) + nMin;
		}

		static double fitClamped(double v, double oMin, double oMax, double nMin, double nMax)
		{
			//division by zero protection
			if(oMin==oMax || nMin==nMax) return nMin;
			if(oMin>oMax) std::swap(oMin, oMax);
			if(v>oMax) v = oMax;
			if(v<oMin) v = oMin;
			return ((v-oMin)/(oMax-oMin)*(nMax-nMin)) + nMin;
		}

		static double clamp(double v, double mn, double mx) {return std::max(std::min(v, mx), mn);}
		static double degrees(double rad) {return 180*rad/M_PI;}
		static double radians(double deg) {return M_PI*deg/180;}



		// --============== general utilities ===============--
		static std::string getTempDir()
		{
			std::string r;
			r = getenv("TEMP");
			if(!r.length()) r = "./";
			return r;
		}

		static std::string getTime()
		{
			time_t curt;
			time(&curt);
			std::string s(ctime(&curt));
			Util::chomp(s);
			return s;
		}

		// bite off trailing '\n' in std::string
		static void chomp(std::string &s)
		{
			if(!s.empty() && s[s.length()-1] == '\n') s.erase(s.length()-1);
		}



		template <class T> static void prVec(std::vector<T>& vec)
		{
			int cntr = 0;
			for(auto e : vec)
			{
				cout << e;
				if(cntr++) { cout << ", "; }
			}
			cout << endl;
		}

		template <class T> static std::string i2s(T i) { std::stringstream s; s << i; return s.str(); }
		static int s2i(const std::string &s) { return std::stoi(s); }
		static float s2f(const std::string &s) { return std::stof(s); }
		static double s2d(const std::string &s) { return std::stod(s); }
		double truncateDouble(const double &v, const jint &digits) { double f = pow(10, digits); return ((int)(v*f))/f; }


		// CLIPBOARD
		void toClip(const std::string &s)
		{
			OpenClipboard(0);
			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
			if(!hg) {CloseClipboard(); return;}
			memcpy(GlobalLock(hg), s.c_str(), s.size());
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
			CloseClipboard();
			GlobalFree(hg);
		}

		inline static void dtrace()
		{
				std::cout << __FILE__ << ", line " << __LINE__ << ": " << __FUNCTION__ << "()" << std::endl;
		}
		inline static void mtrace(const std::string &msg)
		{
				std::cout << __FILE__ << ", line " << __LINE__ << ": " << __FUNCTION__ << "(): " << msg << std::endl;
		}

		template <class T> static bool isNaN(T n) {if(n != n) return true; else return false;}
		template <class T> T clamp(T val, T _min, T _max) const {return max(_min, min(val, _max));}

		// --============== winapi ===============--

		bool Util::fileExists(const std::string& fname)
		{
			std::ifstream infile(fname);
			return infile.good();
		}

		/*
		static int Util::ls(std::vector<std::string> &files, std::string dir=std::string(".") )
		{
			DIR *dp;
			struct dirent *dirp;
			if((dp  = opendir(dir.c_str())) == NULL) { std::cout << "Error(" << errno << ") opening " << dir << endl; return errno; }
			while((dirp = readdir(dp)) != NULL) { files.push_back(string(dirp->d_name)); }
			closedir(dp);
			return 0;
		}
		/**/

		static jint ls(std::vector<std::string> &list, const std::string &mask)
		{
			WIN32_FIND_DATA fd;
			HANDLE h = FindFirstFile(mask.c_str(), &fd);
			if(h == INVALID_HANDLE_VALUE) return 0;
			while(1)
			{
				list.push_back(fd.cFileName);
				if(FindNextFile(h, &fd) == FALSE) break;
			}
			return (jint)list.size();
		}

		static std::vector<std::string> Util::fileDialog(const std::string& path, const std::vector<std::string>& masks)
		{
			OPENFILENAME ofn;		// common dialog box structure
			char cfile[1024];		// buffer for file names
			std::string fullMask;
			for(auto &m : masks) fullMask += m+'\0'+m+'\0';

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = cfile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(cfile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
			ofn.lpstrFilter = fullMask.c_str();
			ofn.nFilterIndex = 1;

			std::vector<std::string> r;
			if(GetOpenFileName(&ofn) != FALSE)	// in this case TRUE can be whatever they want - 1, 2, 17, 32768
			{
				r = Util::char2strings(ofn.lpstrFile);
				if(r.size() > 1)
				{
					std::string dir = r[0];
					r.erase(r.begin());
					for(auto &f : r) f = dir+f;
				}
			}
			return r;
		}

		// converts double zero-terminated and zero-separated char array to vector<strings>, for fileDialog()/winapi
		static std::vector<std::string> char2strings(const char* in)
		{
			std::vector<std::string> r;
			int s=0, e=0;
			const char* c = in;
			while(*c)
			{
				std::string str;
				while(*c) str.push_back(*c++);
				r.push_back(str);
				c++;
			}
			return r;
		}








// ---=== end of jutil::
};




#endif
