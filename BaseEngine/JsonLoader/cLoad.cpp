#include "cLoad.h"



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

	// Cube map Placement guide
	// parameter 1 - positive x (right)
	// parameter 2 - negative x (left)
	// parameter 3 - positive y (top)
	// parameter 4 - negative y (bottom)
	// parameter 5 - positive z (front)
	// parameter 6 - negative z (back)
	//
	// invert the image if the image is inverted
	
	if (g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
		"posx.bmp", "negx.bmp",
		"posy.bmp", "negy.bmp",
		"posz.bmp", "negz.bmp", true, error_string))
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

void LoadingGameObjects(std::vector<cGameObject*>& g_vec_pGameObjects, GLuint& shader_program_ID)
{
	cVAOManager* p_vao_manager = cVAOManager::getInstance();
	size_t num_gameobjects = document["GameObjects"].Size();
	for (size_t c = 0; c < num_gameobjects; c++)
	{
		cGameObject* gameobject = new cGameObject();
		rapidjson::Value& jgameobj = document["GameObjects"][c];
		gameobject->physicsShapeType = (eShapeTypes)jgameobj["physicsshapetype"].GetInt();

		gameobject->meshName = jgameobj["meshname"].GetString();
		gameobject->friendlyName = jgameobj["friendlyname"].GetString();
		gameobject->m_position = glm::vec3(jgameobj["position"]["x"].GetFloat(),
			jgameobj["position"]["y"].GetFloat(),
			jgameobj["position"]["z"].GetFloat());
		gameobject->setOrientation(glm::vec3(
			jgameobj["rotation"]["x"].GetFloat(),
			jgameobj["rotation"]["y"].GetFloat(),
			jgameobj["rotation"]["z"].GetFloat()));
		gameobject->scale = jgameobj["scale"].GetFloat();
		gameobject->objectColourRGBA = glm::vec4(jgameobj["objectcolor"]["r"].GetFloat(),
			jgameobj["objectcolor"]["g"].GetFloat(),
			jgameobj["objectcolor"]["b"].GetFloat(),
			jgameobj["objectcolor"]["a"].GetFloat());
		gameobject->isWireframe = jgameobj["isWireframe"].GetInt();
		gameobject->debugColour = glm::vec4(jgameobj["debugcolor"]["r"].GetFloat(),
			jgameobj["debugcolor"]["g"].GetFloat(),
			jgameobj["debugcolor"]["b"].GetFloat(),
			jgameobj["debugcolor"]["a"].GetFloat());
		for (int i = 0; i < jgameobj["tex"].Size(); i++)
		{
			gameobject->textures[i] = jgameobj["tex"][i].GetString();
			gameobject->textureRatio[i] = jgameobj["texratio"][i].GetFloat();
		}
		gameobject->objectType = (cGameObject::eObjectType)jgameobj["objectype"].GetInt();
		//gameobject->GameObjectMesh = findMeshByName(vec_model_mesh, gameobject->meshName);
		gameobject->SPHERE_radius = jgameobj["sphereRadius"].GetFloat();
		gameobject->collision_radius = jgameobj["bulletCollisionRadius"].GetFloat();
		gameobject->isVisible = jgameobj["isVisible"].GetInt();
		gameobject->is_static = jgameobj["isStatic"].GetInt();

		if (jgameobj["isSkinnedMesh"].GetInt() == 1)
		{
			gameobject->p_skinned_mesh = new cSimpleAssimpSkinnedMesh();
			gameobject->p_skinned_mesh->LoadMeshFromFile(jgameobj["meshname"].GetString(), jgameobj["meshfile"].GetString());
			sModelDrawInfo* pDI = gameobject->p_skinned_mesh->CreateModelDrawInfoObjectFromCurrentModel();
			if (pDI)
			{
				p_vao_manager->LoadModelDrawInfoIntoVAO(*pDI, shader_program_ID);
			}
			size_t num_of_animations = jgameobj["animations"].Size();
			for (size_t c = 0; c < num_of_animations; c++)
			{
				gameobject->p_skinned_mesh->LoadMeshAnimation(jgameobj["animationName"][c].GetString(),
					jgameobj["animations"][c].GetString());
			}
			gameobject->p_skinned_mesh->defaultAnimation = gameobject->p_skinned_mesh->FindAnimationByFriendlyName(
				jgameobj["defaultAnimation"].GetString());
			gameobject->pAniState = new cAnimationState();
			//Animation details
			gameobject->pAniState->defaultAnimation.name = gameobject->p_skinned_mesh->defaultAnimation.friendlyName;
			gameobject->pAniState->defaultAnimation.totalTime = gameobject->p_skinned_mesh->FindAnimationTotalTime(
				gameobject->pAniState->defaultAnimation.name);
			//gameobject->pAniState->defaultAnimation.frameStepTime = gameobject->p_skinned_mesh->FindAnimationFramesPerSecond(gameobject->pAniState->defaultAnimation.name) / 100;
		}
		g_vec_pGameObjects.push_back(gameobject);
	}

}

void LoadStuff(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID, cBasicTextureManager* g_pTextureManager, std::vector<cGameObject*>& g_vec_pGameObjects)
{
	LoadModelMeshDataIntoVector(vec_mesh);
	LoadShadersFromJson(shader_program_ID);
	LoadModelIntoVAO(vec_mesh, shader_program_ID);
	LoadTextures(g_pTextureManager);
	LoadingGameObjects(g_vec_pGameObjects, shader_program_ID);
}
