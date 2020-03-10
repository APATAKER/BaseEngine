#include "cLoad.h"

#include "../ModelLoadingAndVAO/cModelLoader.h"
#include "../ModelLoadingAndVAO/cVAOManager.h"
#include "../ModelLoadingAndVAO/sModelDrawInfo.h"
#include "../shader/cShaderManager.h"

rapidjson::Document document;

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

void LoadModelMeshDataIntoVector(std::vector<cMesh> &vec_mesh)
{
	size_t num_of_models = document["models"].Size();
	std::string error_string = "";
	cModelLoader* p_model_loader = new cModelLoader();

	// Models Loaded here	


	for (size_t c = 0; c < num_of_models; c++)
	{
		cMesh Mesh;
		if (p_model_loader->LoadModel_Assimp(document["models"][c].GetString(), Mesh, error_string))
		{
			std::cout << document["models"][c].GetString() << " model loaded" << std::endl;
			Mesh.meshname = document["MeshName"][c].GetString();
			vec_mesh.push_back(Mesh);
		}
		else
		{
			std::cout << "\nerror:" << error_string << std::endl;
		}
	}
	delete p_model_loader;
}

void LoadShadersFromJson(GLuint& shader_program_ID)
{
	cShaderManager* p_shader_manager = new cShaderManager();
	cShaderManager::cShader vertex_shader;
	vertex_shader.fileName = document["shaders"]["vert"].GetString();
	cShaderManager::cShader fragment_shader;
	fragment_shader.fileName = document["shaders"]["frag"].GetString();
	if (!p_shader_manager->createProgramFromFile("SimpleShader", vertex_shader, fragment_shader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << p_shader_manager->getLastError();
		exit(0);
		//return -1;
	}
	shader_program_ID = p_shader_manager->getIDFromFriendlyName("SimpleShader");
	delete p_shader_manager;
}

void LoadTextures(cBasicTextureManager* g_pTextureManager)
{
	std::string error_string = "";
	g_pTextureManager = cBasicTextureManager::getInstance();
	g_pTextureManager->SetBasePath("assets/textures");
	// Normal Textures
	size_t numTex = document["Textures"].Size();
	for (size_t c = 0; c < numTex; c++)
	{
		if (!g_pTextureManager->Create2DTextureFromBMPFile(document["Textures"][c].GetString(), true))
		{
			std::cout << "\nDidn't load texture" << std::endl;
		}
	}
	// CubeMap Texture
	g_pTextureManager->SetBasePath("assets/textures/cubemaps/");

	if (g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
		"SpaceBox_right1_posX.bmp", "SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp", "SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp", true, error_string))
		std::cout << "\nSpace skybox loaded" << std::endl;
	else
		std::cout << "\nskybox error: " << error_string << std::endl;
}

void LoadModelIntoVAO(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID)
{
	size_t num_of_models = document["models"].Size(); // getting number of models
	cVAOManager* p_vao_manager = cVAOManager::getInstance();   // Singleton Here
	for (size_t c = 0; c < num_of_models; c++)
	{
		sModelDrawInfo draw_info;
		p_vao_manager->LoadModelIntoVAO(document["MeshName"][c].GetString(), vec_mesh[c], draw_info, shader_program_ID);
	}
}

void LoadStuff(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID, cBasicTextureManager* g_pTextureManager)
{
	LoadModelMeshDataIntoVector(vec_mesh);
	LoadShadersFromJson(shader_program_ID);
	LoadModelIntoVAO(vec_mesh, shader_program_ID);
	LoadTextures(g_pTextureManager);
}
