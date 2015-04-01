#include "parsed_objects.h"
#include <glm/gtc/type_ptr.hpp>

using namespace renderlib;

ParsedGeometry::ParsedGeometry():
	is_ref(false), type(0)
{
}

ParsedGeometry::ParsedGeometry(const RGBA& _color, uint8_t _type, const char* _data):
	is_ref(false), color(_color), type(_type)
{
	ctm.decode(_data);
}

ParsedGeometry::ParsedGeometry(const char* _ref, const float* _matrix):
	is_ref(true), ref(_ref), matrix(glm::make_mat4(_matrix))
{
}

ParsedGeometry::~ParsedGeometry()
{
}

ParsedElement::ParsedElement():
	type(0)
{
}

ParsedElement::~ParsedElement()
{
	for (auto geom : geometries)
		delete geom;
	geometries.clear();
}

SharedGeometries::SharedGeometries()
{
}

SharedGeometries::~SharedGeometries()
{
	clear();
}

void SharedGeometries::addSharedGeom(const SharedGeomId& id, const ParsedGeometryPtrList& geoms)
{
	shared[id] = geoms;
}

bool SharedGeometries::getSharedGeom(const SharedGeomId& id, ParsedGeometryPtrList& geoms)
{
	auto it = shared.find(id);
	if (it == shared.end())
		return false;

	geoms = it->second;
	return true;
}

void SharedGeometries::clear()
{
	for (auto it : shared)
		for (auto geom : it.second)
			delete geom;
	shared.clear();
}