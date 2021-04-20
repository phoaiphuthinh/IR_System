/**
 * The microtimer class is used to instantiate a high-resolution microsecond
 * timer. This is a relative timer--i.e. it cannot be used to tell you the time
 * of day, month, or year, only the number of microseconds which have passed
 * between two events. A microsecond is one millionth of a second.
 *
 * Usage example:
 *
 *   microtimer            microtime;
 *   microtimer::size_type start, end;
 *
 *   start = microtime();
 *
 *       // Code to time here
 *
 *   end   = microtime();
 *
 *   std::cout << (end - start) << "Micros." << std::endl;
 *
 * @author  Shane Gavin <http://nodehead.com>
 * @version 1.0
 * @license None (public domain)
 */


 //#############################################################################
 // Dependencies
 //#############################################################################

#include <ctime>
#include "microtimer.h"

#ifdef _WIN32

#include <windows.h>

#endif


//#############################################################################
// Constructor
//#############################################################################

microtimer::microtimer()
{
	/**
	 * Query the CPU ticks per second, covert to per micros and store. Note,
	 * despite naming, this is not the 'actual' CPU frequency.
	 */
#ifdef _WIN32

	QueryPerformanceFrequency((LARGE_INTEGER*)(&this->frequency));
	this->frequency /= 1000000.0;

#endif
}


//#############################################################################
// Functor
//#############################################################################

microtimer::size_type microtimer::operator()()
{

#ifdef _WIN32

	QueryPerformanceCounter((LARGE_INTEGER*)(&this->counter));
	this->counter /= this->frequency;
	return (size_type)this->counter;

#else

	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);

	this->uptime = t.tv_sec;
	this->uptime *= 1000000;
	this->uptime += t.tv_nsec / 1000;

	return this->uptime;

#endif
}