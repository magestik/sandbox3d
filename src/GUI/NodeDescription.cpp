#include "NodeDescription.h"

#include <assert.h>
#include <string.h>

#include <jansson.h>

static NodeDescriptor::NodeType NodeTypeFromString(const char * str)
{
	if (!strcmp(str, "Renderer"))
	{
		return(NodeDescriptor::Renderer);
	}
	else if (!strcmp(str, "FullscreenEffect"))
	{
		return(NodeDescriptor::FullscreenEffect);
	}

	return(NodeDescriptor::UnknownNodeType);
}

static NodeDescriptor::DataType DataTypeFromString(const char * str)
{
	if (!strcmp(str, "Texture"))
	{
		return(NodeDescriptor::Texture);
	}
	else if (!strcmp(str, "Constant"))
	{
		return(NodeDescriptor::Constant);
	}

	return(NodeDescriptor::UnknownDataType);
}

static bool ImportInputs(json_t * array, std::vector<NodeDescriptor::Input> & inputs)
{
	if (!json_is_array(array))
	{
		return(false);
	}

	size_t index;
	json_t * elmt;

	json_array_foreach(array, index, elmt)
	{
		NodeDescriptor::Input input;

		const char * key;
		json_t * value;

		json_object_foreach(elmt, key, value)
		{
			if (!strcmp(key, "name"))
			{
				input.name = json_string_value(value);
			}
			else if (!strcmp(key, "type"))
			{
				input.type = DataTypeFromString(json_string_value(value));
			}
			else
			{
				return(false);
			}
		}

		inputs.push_back(input);
	}

	return(true);
}

static bool ImportOutputs(json_t * array, std::vector<NodeDescriptor::Output> & outputs)
{
	if (!json_is_array(array))
	{
		return(false);
	}

	size_t index;
	json_t * elmt;

	json_array_foreach(array, index, elmt)
	{
		NodeDescriptor::Output output;

		const char * key;
		json_t * value;

		json_object_foreach(elmt, key, value)
		{
			if (!strcmp(key, "name"))
			{
				output.name = json_string_value(value);
			}
			else if (!strcmp(key, "type"))
			{
				output.type = DataTypeFromString(json_string_value(value));
			}
			else
			{
				return(false);
			}
		}

		outputs.push_back(output);
	}

	return(true);
}

/**
 * @brief NodeDescriptor::NodeDescriptor
 */
NodeDescriptor::NodeDescriptor(void) : name("empty"), type(UnknownNodeType)
{

}

/**
 * @brief NodeDescriptor::loadFromFile
 * @param filename
 * @return
 */
bool NodeDescriptor::loadFromFile(const std::string & filename)
{
	json_error_t error;
	json_t * root = json_load_file(filename.c_str(), 0, &error);

	if (!root)
	{
		return(false); // parse error
	}

	const char * key;
	json_t * value;

	json_object_foreach(root, key, value)
	{
		if (!strcmp(key, "name"))
		{
			name = json_string_value(value);
		}
		else if (!strcmp(key, "type"))
		{
			type = NodeTypeFromString(json_string_value(value));
		}
		else if (!strcmp(key, "inputs"))
		{
			ImportInputs(value, inputs);
		}
		else if (!strcmp(key, "outputs"))
		{
			ImportOutputs(value, outputs);
		}
		else
		{
			return(false);
		}
	}

	return(type != UnknownNodeType);
}
