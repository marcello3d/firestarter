#ifndef __dom_h__
#define __dom_h__

#include <string>
#include <vector>
#include <map>

////////////////////////////////////////////////////////////////////////////////
namespace xdom {

////////////////////////////////////////////////////////////////////////////////
class DomNode
{
public:
	std::string name;

	std::vector<DomNode> children;
	std::vector<std::string> cdata;
	std::map<std::string, std::string> attributes;

	std::string &operator [] (std::string idx)
	{
		return attributes[idx];
	}

	DomNode getFirstNamedChild(std::string name)
	{
		for (size_t i = 0; i < children.size(); ++i)
			if (children[i].name == name)
				return children[i];
		return DomNode();
	}
};

////////////////////////////////////////////////////////////////////////////////
class DomParser
{
private:
	std::string data;
	size_t bufpos;

	inline bool isWhitespace(char w)
	{
		return ((w == ' ') || (w == '\n') || (w == '\r') || (w == '\t'));
	}
	inline bool isAlphanumeric(char w)
	{
		std::string misc(":_-");
		return (((w >= 'a') && (w <= 'z')) || ((w >= 'A') && (w <= 'Z')) || ((w >= '0') && (w <= '9')) || (misc.find(w, 0) != std::string::npos));
	}

	inline void skipWhitespace()
	{
		while((bufpos < data.size()) && isWhitespace(data[bufpos]))
			bufpos ++;
	}
	inline void skipNonWhitespace()
	{
		while((bufpos < data.size()) && !isWhitespace(data[bufpos]))
			bufpos ++;
	}
	std::string grabWord();
public:
	DomParser(std::string s) : data(s), bufpos(0) { }

	DomNode parse();
};

DomNode xmlParseFile(std::string fn);

////////////////////////////////////////////////////////////////////////////////
}

#endif
