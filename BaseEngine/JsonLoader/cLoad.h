#pragma once
#include "../commonHeaders.h"



#include "../GameObject/cGameObject.h"
#include "../ModelLoadingAndVAO/cMesh.h"
#include "../Textures/cBasicTextureManager.h"
#include "../ModelLoadingAndVAO/cModelLoader.h"
#include "../ModelLoadingAndVAO/cVAOManager.h"
#include "../ModelLoadingAndVAO/sModelDrawInfo.h"
#include "../shader/cShaderManager.h"


void InitJson(std::string filename);

void LoadModelMeshDataIntoVector(std::vector<cMesh> &vec_mesh);
void LoadShadersFromJson(GLuint &shader_program_ID);
void LoadTextures(cBasicTextureManager* g_pTextureManager);
void LoadModelIntoVAO(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID);

void LoadingGameObjects(std::vector<cGameObject*> &g_vec_pGameObjects, GLuint& shader_program_ID);

void LoadStuff(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID, cBasicTextureManager* g_pTextureManager, std::vector<cGameObject*>& g_vec_pGameObjects);

class cJSONUtility
{
public:

	
	static rapidjson::Document open_document(std::string& filename);
	

};
