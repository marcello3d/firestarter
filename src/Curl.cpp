#include <curl/curl.h>
#include <cstdio>
#include "Main.h"
#include "Curl.h"


Curl::Curl() {
	curl = curl_easy_init();
}


Curl::~Curl() {
	curl_easy_cleanup(curl);
}

void Curl::download(string url, string file) {
	FILE *f = fopen(file.c_str(), "wb");
	if (f) {
		gLog.log(Log::vDebug, "Reading %s to %s", url.c_str(),file.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
 		curl_easy_perform(curl);
		fclose(f);
		gLog.log(Log::vDebug, "Done reading %s to %s", url.c_str(),file.c_str());
	}
}


size_t Curl::write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	FILE *f = (FILE*)userp;

	fwrite(buffer, size, nmemb, f);

	return size * nmemb;
}
