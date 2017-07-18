////////////////////////////////////////////////////////////////////////////////
#ifndef __resourcemanager_h__
#define __resourcemanager_h__

////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <map>
#include <typeinfo>

#include "resource.h"
#include "log.h"

////////////////////////////////////////////////////////////////////////////////
typedef std::map<std::string, Resource *> ResourceMap;

////////////////////////////////////////////////////////////////////////////////
extern Log gLog;

////////////////////////////////////////////////////////////////////////////////
class ResourceManager
{
private:
	ResourceMap resources;
public:
	ResourceManager();
	~ResourceManager();

	template <typename T> T *load(std::string fn)
	{
		std::string rscname = fn + "*" + typeid(T).name();
		ResourceMap::iterator iter = resources.find(rscname);
		if (iter != resources.end())
		{
			gLog.log(Log::vDebug, "Retrieved cached resource '%s'", rscname.c_str());
			return (T*)(iter->second);
		}
		
		T *rsrc = new T;
		if (!rsrc->load(fn))
		{
			delete rsrc;
			return 0;
		}
		
		gLog.log(Log::vNotice, "Loaded resource '%s'", rscname.c_str());
		
		resources[rscname] = rsrc;
		
		return rsrc;
	}
};

////////////////////////////////////////////////////////////////////////////////
#endif
