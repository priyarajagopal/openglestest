#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "color.h"
#include "ctm_decoder.h"
#include "element_id.h"
#include <glm/glm.hpp>

namespace renderlib {

class ParsedGeometry
{
public:
	ParsedGeometry();
	ParsedGeometry(const RGBA& _color, uint8_t _type, const char* _data);
	ParsedGeometry(const char* _ref, const float* _matrix);
	~ParsedGeometry();

	RGBA color;
	uint8_t type;	// 0: triangles, 1: lines
	CTMDecoder ctm;

	// for ref
	bool is_ref;
	std::string ref;
	glm::mat4 matrix;

};

typedef ParsedGeometry* ParsedGeometryPtr;
typedef std::vector<ParsedGeometryPtr> ParsedGeometryPtrList;

class ParsedElement
{
public:
	ParsedElement();
	~ParsedElement();
	ElementId element_id;
	ParsedGeometryPtrList geometries;
};
typedef ParsedElement* ParsedElementPtr;

typedef std::map<ElementId, ParsedGeometryPtrList> SharedGeometriesMap;
class SharedGeometries
{
public:
	SharedGeometries();
	~SharedGeometries();
	void addSharedGeom(const ElementId& id, const ParsedGeometryPtrList& geoms);
	bool getSharedGeom(const ElementId& id, ParsedGeometryPtrList& geoms);
	void clear();

private:
	SharedGeometriesMap shared;
};

}

