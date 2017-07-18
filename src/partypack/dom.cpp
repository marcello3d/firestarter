#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "dom.h"

#define DOM_DEBUG 0

////////////////////////////////////////////////////////////////////////////////
namespace xdom {
using namespace std;

////////////////////////////////////////////////////////////////////////////////
const char open_tag = '<';
const char end_tag = '>';
const char singular_tag = '/';
const char closing_tag = '/';
const char attr_eq = '=';

char cheaptolower(char c)
{
	if (c >= 'A' && c <= 'Z')
		return 'a' + c - 'A';
	return c;
}

////////////////////////////////////////////////////////////////////////////////
string DomParser::grabWord()
{
	skipWhitespace();
	
	if (bufpos >= data.size())
		return "";
	
	int start = bufpos ++;
	int end = bufpos;
	
	if ((data[start] == '\"') || (data[start] == '\''))
	{
		start ++;
		while ((data[bufpos] != data[start-1]) && (bufpos < data.size()))
			bufpos ++;
		end = bufpos ++;
	}
	else if (isAlphanumeric(data[start]))
	{
		bufpos--;
		while (isAlphanumeric(data[bufpos]))
			bufpos ++;
		
		end = bufpos;
	}
	else
		return "majorborkup";
	
	return data.substr(start, end - start);
}

////////////////////////////////////////////////////////////////////////////////
DomNode DomParser::parse()
{
	DomNode tmp;

#if DOM_DEBUG
	printf("+++ beginning to parse tag\n");
#endif
	
	bufpos = data.find(open_tag, bufpos);
	// Make sure it's actually a tag ...
	if (data[bufpos] != open_tag)
	{
#if DOM_DEBUG
		printf("+++ blah! malformed XML '%c'\n", data[bufpos]);
#endif
		// Whoops
	}
	bufpos ++;
	
	// Read name
	tmp.name = grabWord();
	transform(tmp.name.begin(), tmp.name.end(), tmp.name.begin(), cheaptolower);
	
#if DOM_DEBUG
	printf("+++ name: '%s'\n", tmp.name.c_str()); //tmp.name.c_str());
#endif
	
	// Parse attributes
	while(true)
	{
		skipWhitespace();
		
		if (data[bufpos] == singular_tag)
		{
			// Skip the last two and just assume that an end_tag follows it
			bufpos += 2;
#if DOM_DEBUG
			printf("+++ tag is singular.\n");
#endif			
			return tmp;
		}
		else if (data[bufpos] == end_tag)
		{
#if DOM_DEBUG
			printf("+++ end of opening tag.\n");
#endif
			break;
		}
		else
		{
			string vname = grabWord();
			transform(vname.begin(), vname.end(), vname.begin(), cheaptolower);
			
			skipWhitespace();
			
			if(data[bufpos] == attr_eq)
			{
				bufpos++;
				tmp.attributes[vname] = grabWord();
			}
			else
				tmp.attributes[vname] = "";
			
#if DOM_DEBUG
			printf("+++ grabbed attribute '%s' = '%s'.\n", vname.c_str(), tmp.attributes[vname].c_str());
#endif
		}
	}
	bufpos ++;
	
	while (true)
	{
		// Ryghte, so let's find the next tag
		int bikkit = data.find(open_tag, bufpos);
#if DOM_DEBUG
		printf("+++ found next tag at %d.\n", bikkit);
#endif
		
		tmp.cdata.push_back(data.substr(bufpos, bikkit - bufpos));
		
		if(data[bikkit+1] == closing_tag)
		{
			bufpos = data.find(end_tag, bikkit+1) + 1;
#if DOM_DEBUG
			printf("+++ tag is closing tag.\n");
#endif
			break;
		}
		else
		{
			bufpos = bikkit;
#if DOM_DEBUG
			printf("+++ tag is child node.\n");
#endif			
			DomNode child = parse();
			tmp.children.push_back(child);
		}
	}
	
	return tmp;
}

////////////////////////////////////////////////////////////////////////////////
DomNode xmlParseFile(string fn)
{
	string result("");
	
	char buffer[4096];
	FILE *fp = fopen(fn.c_str(), "r");
	while(fgets(buffer, 4096, fp))
	{
		result += buffer;
	}
	fclose(fp);
	
	DomParser parser = DomParser(result);
	return parser.parse();
}

////////////////////////////////////////////////////////////////////////////////
}
