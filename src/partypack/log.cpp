////////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <cstdarg>
#include <ctime>

#include "log.h"

////////////////////////////////////////////////////////////////////////////////
using namespace std;

////////////////////////////////////////////////////////////////////////////////
/*
* Ctor
*/
Log::Log() : verbosity(0)
{
}

/*
* Ctor again
*/
Log::Log(std::string fn, int _verbosity) : verbosity(0)
{
	start(fn, _verbosity);
}

////////////////////////////////////////////////////////////////////////////////
/*
* Manual starting of the log
*/
void Log::start(std::string fn, int _verbosity)
{
	filename = fn;
	verbosity = _verbosity;

	time_t now = time(0);
	static char nows[128];
	strftime(nows, sizeof(nows), "%Y/%m/%d %H:%M:%S", localtime(&now));

	fstream stream(filename.c_str(), ios_base::out | ios_base::app);
	stream << endl;
	stream.close();

	log(vSystem, "Logging started %s with verbosity level %d", nows, verbosity);
}

////////////////////////////////////////////////////////////////////////////////
/*
* Formatted logging function
*/
void Log::log(int mverbosity, const char *str, ...)
{
	if (mverbosity > verbosity)
		return;
	if (filename.length() == 0)
		return;

	static char buf[4096];

	va_list ap;
	va_start(ap, str);
	vsnprintf(buf, sizeof(buf), str, ap);
	va_end(ap);

	fstream stream(filename.c_str(), ios_base::out | ios_base::app);

	if (mverbosity == vSystem)
		stream << "[ SYSTEM] ";
	else if (mverbosity == vError)
		stream << "[  ERROR] ";
	else if (mverbosity == vWarning)
		stream << "[WARNING] ";
	else if (mverbosity == vNotice)
		stream << "[ NOTICE] ";
	else if (mverbosity == vDebug)
		stream << "[  DEBUG] ";
	else
		stream << "[       ]";

	stream << buf << endl;
	stream.close();
}
