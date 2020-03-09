#pragma once
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include <fstream>



void InitJson(std::string filename);

class cJSONUtility
{
public:

	
	static rapidjson::Document open_document(std::string& filename);
	

};
