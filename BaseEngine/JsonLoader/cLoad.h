#pragma once
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include <fstream>

class cJSONUtility
{
public:
	static rapidjson::Document open_document(std::string& filename);
	

};
