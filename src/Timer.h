#ifndef _Timer
#define _Timer

// idea taken from: cplus.about.com/od/howtodothingsin1/a/timing.htm and reimplemented as a class
#include <windows.h>

struct Time
{
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
};

class Timer
{
	public:
		Timer()									{ timer.start.QuadPart = 0; timer.stop.QuadPart = 0; QueryPerformanceFrequency(&frequency); }
		~Timer()								{ stopTimer(); }

		void	startTimer()					{ QueryPerformanceCounter(&timer.start); }
		void	stopTimer()						{ QueryPerformanceCounter(&timer.stop); }
		double	getElapsedTime()				{ LARGE_INTEGER time; time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart; return LIToSecs(time); }
		double	LIToSecs(LARGE_INTEGER & L)		{ return ((double)L.QuadPart /(double)frequency.QuadPart); }

	private:
		Time timer;
		LARGE_INTEGER frequency;
};

#ifndef TIMER4_START
	#define TIMERA_START		Timer timerA;timerA.startTimer();
	#define TIMERA_END			timerA.stopTimer(); std::cerr << timerA.getElapsedTime() << "s";
	#define TIMERA_ENDM(msg)	timerA.stopTimer(); std::cerr << msg << ": " << timerA.getElapsedTime() << "s" << std::endl;

	#define TIMERB_START		Timer timerB;timerB.startTimer();
	#define TIMERB_END			timerB.stopTimer(); std::cerr << timerB.getElapsedTime() << "s";
	#define TIMERB_ENDM(msg)	timerB.stopTimer(); std::cerr << msg << timerB.getElapsedTime() << "s" << std::endl;

	#define TIMER1_START		Timer timer1;timer1.startTimer();
	#define TIMER1_END			timer1.stopTimer(); std::cerr << timer1.getElapsedTime() << "s";
	#define TIMER1_ENDM(msg)	timer1.stopTimer(); std::cerr << msg << ": " << timer1.getElapsedTime() << "s" << std::endl;

	#define TIMER2_START		Timer timer2;timer2.startTimer();
	#define TIMER2_END			timer2.stopTimer(); std::cerr << timer2.getElapsedTime() << "s";
	#define TIMER2_ENDM(msg)	timer2.stopTimer(); std::cerr << msg << ": " << timer2.getElapsedTime() << "s" << std::endl;

	#define TIMER3_START		Timer timer3;timer3.startTimer();
	#define TIMER3_END			timer3.stopTimer(); std::cerr << timer3.getElapsedTime() << "s";
	#define TIMER3_ENDM(msg)	timer3.stopTimer(); std::cerr << msg << ": " << timer3.getElapsedTime() << "s" << std::endl;

	#define TIMER4_START		Timer timer4;timer4.startTimer();
	#define TIMER4_END			timer4.stopTimer(); std::cerr << timer4.getElapsedTime() << "s";
	#define TIMER4_ENDM(msg)	timer4.stopTimer(); std::cerr << msg << ": " << timer4.getElapsedTime() << "s" << std::endl;
#endif

#endif