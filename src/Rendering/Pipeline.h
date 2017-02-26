#pragma once

#include <GPU.h>

#include <Vector.h>
#include <Matrix.h>

#include <vector>
#include <string>
#include <map>

#include "RHI/RHI.h"

namespace tinyxml2
{
	class XMLElement;
}

class Rendering;

class Pipeline
{
public:

	Pipeline();
	Pipeline(const tinyxml2::XMLElement * element, const Rendering & rendering);


	std::map<std::string, GLuint> m_mapSamplers;
};
