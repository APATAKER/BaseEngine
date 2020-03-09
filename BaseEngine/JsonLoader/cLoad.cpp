#include "cLoad.h"

extern rapidjson::Document document;

rapidjson::Document cJSONUtility::open_document(std::string& filename)
{
	std::ifstream input_file(filename);
	if (!input_file.is_open())
	{
		std::cout << "Can't open Json file" << std::endl;
		return nullptr;
	}

	rapidjson::IStreamWrapper isw(input_file);
	rapidjson::Document doc;
	doc.ParseStream(isw);

	return doc;
}

void InitJson(std::string filename)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	std::string jsonFilename = "Config/config.json";
	document = cJSONUtility::open_document(filename);
}
