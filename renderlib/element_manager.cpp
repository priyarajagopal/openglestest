#include "element_manager.h"
#include <memory>
#include <sstream>
#include "rapidjson/document.h"
#include "logging.h"

using namespace renderlib;

ElementManager::ElementManager()
{
}


ElementManager::~ElementManager()
{
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
				jGeom.HasMember("color") ? RGBA(jGeom["color"].GetUint()) : RGBA(255, 255, 255, 255),
				jGeom["type"].GetInt(),
				jGeom["data"].GetString());
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

bool ElementManager::load_model_data(const char* data)
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
			const char* elementId = jSharedGeom["id"].GetString();
			LOG(">>SharedGeom id[%s]", elementId);
			ParsedGeometryPtrList geometries = parseJsonGeometryArray(jSharedGeom["geometry"]);
			shared_geoms.addSharedGeom(elementId, geometries);
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
			const char* elementId = jElement["elementId"].GetString();

			LOG(">>Element id[%s]", elementId);
			elem->element_id = elementId;
			elem->geometries = parseJsonGeometryArray(jElement["geometry"]);
			parsed_elements.push_back(elem);
		}
	}

	processParsedElements();

	return true;
}

void ElementManager::addGeomToBuffer(BufferPtr& buffer, const ParsedGeometryPtr& geom, const glm::mat4& mat)
{
	bool added = false;
	while (!added)
	{
		added = buffer->add_ctm_data(geom->ctm, mat, geom->color, bbox);
		if (!added)
		{
			buffer->load_buffers();
			buffers.push_back(buffer);
			buffer = std::make_shared<Buffer>();
		}
	}
}

void ElementManager::processParsedElements()
{
	auto currentBuffer = std::make_shared<Buffer>();

	while (!parsed_elements.empty())
	{
		auto element = parsed_elements.front();

		for (auto& geom : element->geometries)
		{
			if (geom->is_ref)
			{
				ParsedGeometryPtrList geoms;
				if (shared_geoms.getSharedGeom(geom->ref, geoms))
				{
					for (auto& shared_geom : geoms)
						addGeomToBuffer(currentBuffer, shared_geom, geom->matrix);
				}
			}
			else
			{
				addGeomToBuffer(currentBuffer, geom, glm::mat4(1.0));
			}
		}

		delete element;
		parsed_elements.pop_front();
	}

	if (!currentBuffer->is_loaded())
	{
		currentBuffer->load_buffers();
		buffers.push_back(currentBuffer);
	}

	shared_geoms.clear();
}