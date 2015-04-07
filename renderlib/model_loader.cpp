#include "model_loader.h"
#include "element_manager.h"
#include <sstream>
#include "rapidjson/document.h"
#include "curl/curl.h"
#include "logging.h"

using namespace renderlib;

ModelLoader::ModelLoader(ElementManager* element_manager_):
	element_manager(element_manager_)
{
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::load_url(const char* url_)
{
	url = url_;
	thread = std::thread(&ModelLoader::thread_load_url, this);
}

bool ModelLoader::wait_until_done()
{
	thread.join();
	return true;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *user_data)
{
	std::stringstream* os = (std::stringstream*) user_data;

	size_t totsize = size * nmemb;
	os->write((const char*)buffer, totsize);

	return totsize;
}

void ModelLoader::thread_load_url()
{
	std::stringstream ss;
	CURL* hc = curl_easy_init();
	curl_easy_setopt(hc, CURLOPT_URL, url.c_str());
	curl_easy_setopt(hc, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(hc, CURLOPT_WRITEDATA, &ss);

	CURLcode rc = curl_easy_perform(hc);
	long code = 0;
	curl_easy_getinfo(hc, CURLINFO_RESPONSE_CODE, &code);

	curl_easy_cleanup(hc);

	load_model_data(ss.str().c_str());
}

ParsedGeometryPtr parseJsonGeometry(rapidjson::Value& jGeom)
{
	ParsedGeometryPtr parsedGeom = nullptr;
	if (jGeom.HasMember("ref"))	
	{
		auto& jMatrix = jGeom["matrix"];
		float mat[16];
		for (int i=0; i < 16; i++)
			mat[i] = jMatrix[i].GetDouble();
		parsedGeom = new ParsedGeometry(jGeom["ref"].GetString(), mat);

		//LOG(">>>>geom ref[%s]", parsedGeom->ref);
		//LOG(">>>>geom matrix[%s]", ss.str().c_str());
	}
	else 
	{
		int type = jGeom["type"].GetInt();
		if (type == 0)	// ignore lines for now
		{
			parsedGeom = new ParsedGeometry(
				jGeom.HasMember("color") ? RGBA(jGeom["color"].GetInt()) : RGBA(255, 255, 255, 255),
				jGeom["type"].GetInt(),
				jGeom["data"].GetString()
			);
		}

		//LOG(">>>>geom type[%d]", parsedGeom->type);
		//LOG(">>>>geom color[%d,%d,%d,%d]", parsedGeom->color.r(), parsedGeom->color.g(), parsedGeom->color.b(), parsedGeom->color.a());
	}
	return parsedGeom;
}

ParsedGeometryPtrList parseJsonGeometryArray(rapidjson::Value& jGeoms)
{
	ParsedGeometryPtrList geometries;
	for (rapidjson::SizeType j = 0; j < jGeoms.Size(); j++)	
	{
		auto& jGeom = jGeoms[j];
		ParsedGeometryPtr parsedGeom = parseJsonGeometry(jGeom);
		if (parsedGeom)
			geometries.push_back(parsedGeom);
	}
	return geometries;
}

bool ModelLoader::load_model_data(const char* data)
{
	rapidjson::Document doc;
	doc.Parse(data);

	if (doc.HasMember("sharedGeometries"))
	{
		LOGW("Parsing sharedGeometries");
		auto& jSharedGeometries = doc["sharedGeometries"];
		for (rapidjson::SizeType i = 0; i < jSharedGeometries.Size(); i++)
		{
			auto& jSharedGeom = jSharedGeometries[i];
			const char* id = jSharedGeom["id"].GetString();
			ParsedGeometryPtrList geometries = parseJsonGeometryArray(jSharedGeom["geometry"]);
			element_manager->add_shared_geom(id, geometries);
		}
	}

	if (doc.HasMember("geometries"))
	{
		LOGW("Parsing elements");
		auto& jElements = doc["geometries"];
		for (rapidjson::SizeType i = 0; i < jElements.Size(); i++)
		{
			ParsedElementPtr elem = new ParsedElement();
			auto& jElement = jElements[i];
			ElementId elementId = jElement["elementId"].GetUint();

			elem->element_id = elementId;
			if (jElement.HasMember("type"))
				elem->type = jElement["type"].GetInt();
			elem->geometries = parseJsonGeometryArray(jElement["geometry"]);

			element_manager->queue_parse_element(elem);
		}
	}

	return true;
}