#include "element_manager.h"
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include "logging.h"
#include "model_loader.h"
#include "viewer.h"
#include "geom_info.h"

using namespace renderlib;

#define LOAD_DURING_RENDER_MAX_DURATION 20

ElementManager::ElementManager(Viewer* viewer_):
	current_opaque_buffer(nullptr), current_transparent_buffer(nullptr),
	loading(false), viewer(viewer_)
{
}

ElementManager::~ElementManager()
{
}

std::mutex parsed_elements_mutex;

void ElementManager::add_shared_geom(const SharedGeomId& id, const ParsedGeometryPtrList& geometries)
{
	shared_geoms.addSharedGeom(id, geometries);
}

void ElementManager::queue_parse_element(ParsedElementPtr elem)
{
	std::lock_guard<std::mutex> lock(parsed_elements_mutex);
	parsed_elements.push_back(elem);
}

void ElementManager::set_loading(bool l)
{
	loading = l;
}

void ElementManager::load_model(const char* url)
{
	std::thread t(&ElementManager::thread_load_model, this, url);
	t.detach();
}

void ElementManager::thread_load_model(const std::string url)
{
	set_loading(true);
	GeomInfo ginfo = GeomInfo::get_from_url(url);
	viewer->fit_camera_to_box(ginfo.bbox.minpos.x, ginfo.bbox.minpos.y, ginfo.bbox.minpos.z,
		ginfo.bbox.maxpos.x, ginfo.bbox.maxpos.y, ginfo.bbox.maxpos.z);

	while (!ginfo.urls.empty())
	{
		std::string file_url = ginfo.urls.front();
		ginfo.urls.pop_front();

		ModelLoader loader(this);
		loader.load_url(file_url.c_str());
		loader.wait_until_done();
	}
	
	set_loading(false);
}

BufferPtr* ElementManager::allocate_buffer(bool transparent)
{
	BufferPtr* buffer = transparent ? &current_transparent_buffer : &current_opaque_buffer;
	if (*buffer == nullptr)
		*buffer = std::make_shared<Buffer>();
	return buffer;
}

BufferOffset ElementManager::add_geom_to_buffer(const ParsedGeometryPtr& geom, const glm::mat4& mat)
{
	BufferPtr* buffer = allocate_buffer(geom->color.is_transparent());
	BufferOffset offset;

	bool added = false;
	while (!added)
	{
		added = (*buffer)->add_ctm_data(geom->ctm, mat, geom->color, bbox, offset);
		if (added)
			offset.buffer = *buffer;
		else
		{
			(*buffer)->load_buffers();
			buffers.push_back(*buffer);
			*buffer = nullptr;
			buffer = allocate_buffer(geom->color.is_transparent());
		}
	}
	
	return offset;
}

ParsedElementPtr ElementManager::get_next_parsed_element()
{
	std::lock_guard<std::mutex> lock(parsed_elements_mutex);
	if (parsed_elements.empty())
		return nullptr;
	else
	{
		ParsedElementPtr element = parsed_elements.front();
		parsed_elements.pop_front();
		return element;
	}
}

void ElementManager::process_parsed_elements()
{
	auto start = std::chrono::high_resolution_clock::now();

	ParsedElementPtr pElement;
	while ((pElement = get_next_parsed_element()) != nullptr)
	{
		ElementPtr element = std::make_shared<Element>(pElement->element_id, pElement->type);

		for (auto& geom : pElement->geometries)
		{
			if (geom->is_ref)
			{
				ParsedGeometryPtrList geoms;
				if (shared_geoms.getSharedGeom(geom->ref, geoms))
				{
					for (auto& shared_geom : geoms)
					{
						GeometryPtr geometry = std::make_shared<Geometry>(shared_geom->color, shared_geom->type);
						geometry->buffer_offset = add_geom_to_buffer(shared_geom, geom->matrix);
						element->geometries.push_back(geometry);
					}
				}
			}
			else
			{
				GeometryPtr geometry = std::make_shared<Geometry>(geom->color, geom->type);
				geometry->buffer_offset = add_geom_to_buffer(geom, glm::mat4(1.0));
				element->geometries.push_back(geometry);
			}
		}
		elements[element->element_id] = element;
		delete pElement;

		auto end = std::chrono::high_resolution_clock::now();
		auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		if (dur_ms.count() > LOAD_DURING_RENDER_MAX_DURATION)
			break;
	}

	// this section must run only when there is no more data coming
	if (!is_loading() && parsed_elements.empty())
	{
		if (current_opaque_buffer && !current_opaque_buffer->is_loaded())
		{
			current_opaque_buffer->load_buffers();
			buffers.push_back(current_opaque_buffer);
			current_opaque_buffer = nullptr;
		}
		if (current_transparent_buffer && !current_transparent_buffer->is_loaded())
		{
			current_transparent_buffer->load_buffers();
			buffers.push_back(current_transparent_buffer);
			current_transparent_buffer = nullptr;
		}
		shared_geoms.clear();
	}
}