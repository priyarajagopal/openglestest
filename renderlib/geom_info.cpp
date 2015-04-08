#include "geom_info.h"
#include <sstream>
#include "rapidjson/document.h"
#include "curl/curl.h"
#include "logging.h"

using namespace renderlib;

GeomInfo::GeomInfo()
{
}

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *user_data)
{
	std::stringstream* os = (std::stringstream*) user_data;

	size_t totsize = size * nmemb;
	os->write((const char*)buffer, totsize);

	return totsize;
}

GeomInfo GeomInfo::get_from_url(const std::string& url)
{
	LOGW("Parsing geomInfo");
	std::stringstream ss;
	CURL* hc = curl_easy_init();
	curl_easy_setopt(hc, CURLOPT_URL, url.c_str());
	curl_easy_setopt(hc, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(hc, CURLOPT_WRITEDATA, &ss);

	CURLcode rc = curl_easy_perform(hc);
	long code = 0;
	curl_easy_getinfo(hc, CURLINFO_RESPONSE_CODE, &code);

	curl_easy_cleanup(hc);

	rapidjson::Document doc;
	doc.Parse(ss.str().c_str());

	float val[6];
	auto& bbox = doc["bbox"];
	for (rapidjson::SizeType i = 0; i < 6; i++)
	{
		auto& jval = bbox[i];
		val[i] = jval.GetDouble();
	}

	GeomInfo ginfo;
	ginfo.bbox = Box(val[0], val[1], val[2], val[3], val[4], val[5]);
	
	std::string prefix = url.substr(0, url.rfind("/") + 1);

	auto& files = doc["outputFiles"];
	for (rapidjson::SizeType i = 0; i < files.Size(); i++)
	{
		auto& jfile = files[i];
		std::string file_url = jfile.GetString();
		if (file_url.find("http") != 0)
			file_url = prefix + file_url;
		ginfo.urls.push_back(file_url);
	}

	return ginfo;
}