#ifndef _CURL_H
#define _CURL_H


#include <string>
using namespace std;

typedef void CURL;

class Curl {
	public:
		Curl();
		~Curl();

		void download(string url, string file);

	private:
		static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

		CURL *curl;
};
#endif
