#pragma once

#include <string>
#include <thread>

namespace renderlib {

class ElementManager;

class ModelLoader
{
public:
	ModelLoader(ElementManager* element_manager);
	~ModelLoader();
	void load_url(const char* url);
	bool wait_until_done();

private:
	ElementManager* element_manager;
	std::string url;
	std::thread thread;

	void thread_load_url();
	bool load_model_data(const char* data);
};

}