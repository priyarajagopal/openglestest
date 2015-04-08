#pragma once

#include "box.h"
#include <deque>
#include <string>

namespace renderlib {

class GeomInfo
{
public:
	static GeomInfo get_from_url(const std::string& url);

	Box bbox;
	std::deque<std::string> urls;

private:
	GeomInfo();
};

}