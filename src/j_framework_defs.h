#ifndef _j_framework_defs
#define _j_framework_defs

#define DEBUG 1
#define J_FRAMEWORK_VERSION "11.aug.2016"

// from CImg
#ifndef	_CRT_SECURE_NO_DEPRECATE
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4311)
	#pragma warning(disable:4312)
	#pragma warning(disable:4512)
	#pragma warning(disable:4571)
	#pragma warning(disable:4640)
	#pragma warning(disable:4706)
	#pragma warning(disable:4710)
	#pragma warning(disable:4800)
	#pragma warning(disable:4804)
	#pragma warning(disable:4820)
	#pragma warning(disable:4996)
	#define _CRT_SECURE_NO_DEPRECATE 1
	#define _CRT_SECURE_NO_WARNINGS 1
	#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif

#ifndef COUT_SET
	#define COUT_SET {cout.setf(ios::fixed); cout.precision(8);}
	#define OSTR_SET {ostr.setf(ios::fixed); ostr.precision(8);}
#endif

#ifndef M_PI
	#define M_PI 3.14159265358979323
#endif

#ifndef loop1
	#define loop1(n)		for(int i=0; i<n; i++)
	#define loop2(n)		for(int j=0; j<n; j++)
	#define loop3(n)		for(int k=0; k<n; k++)
	#define loopu1(n)		for(uint i=0; i<n; i++)
	#define loopu2(n)		for(uint j=0; j<n; j++)
	#define loopu3(n)		for(uint k=0; k<n; k++)
	#define loop12(s, e)	for(int i=s; i<e; i++)
	#define unum(t)			Util::i2s(t)
#endif

#ifndef errstatus
	#define errstatus(status)	\
	if(status != MS::kSuccess)	\
	{							\
		std::string allstr = std::string(__FILE__) + ", " + Util::i2s(__LINE__) + ": " + std::string(__FUNCTION__) + "(): " + std::string(status.errorString().asChar());	\
		MGlobal::displayError(MString(allstr.c_str()));	\
		return status;			\
	}
#endif

// typedefs
typedef unsigned char byte;
typedef unsigned int uint;				// __int32, 4 bytes, –2,147,483,648 to 2,147,483,647
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef long long int lint;				// __int64, 8 bytes, –9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef unsigned long long int ulint;	// unsigned __int64, 8 bytes, 0 to 18,446,744,073,709,551,615
typedef unsigned __int64 uint64;

// for Mesh lib
typedef	float	jreal;
typedef	int		jint;

#endif
