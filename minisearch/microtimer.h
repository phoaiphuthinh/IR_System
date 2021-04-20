#ifndef MICROTIMER_H
#define MICROTIMER_H

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

class microtimer
{
public:

	/**
	 * A type big enough to hold, and perform calculations on, large
	 * microsecond values. long long is an integer type which will be
	 * at least 64 bits in size (possibly larger).
	 */
	typedef unsigned long long size_type;

	/**
	 * For code readability: the numbers microsecond values must be
	 * divided by to convert to other time units. Note: typed enums
	 * (like this one) require C++11 support.
	 */
	enum time_unit : size_type
	{
		TO_MILLIS = 1000,
		TO_SECONDS = TO_MILLIS * 1000,
		TO_MINUTES = TO_SECONDS * 60,
		TO_HOURS = TO_MINUTES * 60,
		TO_DAYS = TO_HOURS * 24
	};

	/**
	 * Constructor.
	 */
	microtimer();

	/**
	 * Get the current system time in microseconds. Important: this time
	 * is meaningless on its own--it is the no. of microseconds which
	 * have passed since an unspecified moment in time (generally, some
	 * stage of the current machine's boot process). The value only has
	 * meaning when used in relation to another value retrieved through
	 * the same call, in the same program run.
	 *
	 * In C++, overloading the parens operator like this means objects of
	 * this type are functors, that is: objects that look like functions.
	 */
	size_type operator()();

	/**
	 * Convert a microsecond value to a time unit defined in the time_unit
	 * enum. Note: precision will be lost. That is: this function will return
	 * the number of full units in the microsecond value. Leftover micros
	 * are ignored.
	 *
	 * The inline modifier asks the compiler to replace calls to this
	 * function with the contents of the function body, to save the overhead
	 * of performing a function call. For small, one-liner functions, like
	 * this, inlining is good practice. However, there is no guarantee the
	 * compiler will comply. Inline is a request, not an order.
	 *
	 * Inline member functions must be implemented in the class header file
	 * so the compiler can see the implementation where a header has been
	 * included.
	 */
	inline static size_type convert(size_type micros, time_unit unit)
	{
		return (micros / unit);
	}


private:

#ifdef _WIN32

	/**
	 * For windows systems, we store the CPU frequency ticks per microsecond.
	 * Double value used to preserve precision until total microseconds are
	 * calculated. counter stores the number of ticks which have passed since
	 * system boot.
	 */
	long double frequency;
	long double counter;

#else

	/**
	 * Naming is loose, the value stored in this member may not represent
	 * system uptime, but will be a microsecond value which has relation
	 * to an unspecified moment in time. The member is mainly used to
	 * store large numbers and facilitate arithmetic on same without
	 * worrying about expression overflows.
	 */
	size_type uptime;

#endif

};


#endif