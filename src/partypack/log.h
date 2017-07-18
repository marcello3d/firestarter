////////////////////////////////////////////////////////////////////////////////
#ifndef __log_h__
#define __log_h__

////////////////////////////////////////////////////////////////////////////////
#include <string>

////////////////////////////////////////////////////////////////////////////////
class Log
{
private:
	std::string filename;
	int verbosity;
public:
	// Ctor
	Log();
	Log(std::string fn, int _verbosity = vWarning);

	// Verbosity levels
	static const int vSystem = 0;
	static const int vError = 1;
	static const int vWarning = 2;
	static const int vNotice = 3;
	static const int vDebug = 4;

	// Set verbosity
	inline void setVerbosity(int _v)
	{
		if (verbosity != _v)
		{
			verbosity = _v;
			log(vNotice, "Log verbosity changed to %d", _v);
		}
	}

	// Log function
	void log(int mverbosity, const char *str, ...);

	// Start it manually
	void start(std::string fn, int _verbosity = vWarning);
};

////////////////////////////////////////////////////////////////////////////////
#endif
