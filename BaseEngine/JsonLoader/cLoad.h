#pragma once
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <iostream>
#include <fstream>
#include <glad/glad.h>


#include "../ModelLoadingAndVAO/cMesh.h"
#include "../Textures/cBasicTextureManager.h"


void InitJson(std::string filename);

void LoadModelMeshDataIntoVector(std::vector<cMesh> &vec_mesh);
void LoadShadersFromJson(GLuint &shader_program_ID);
void LoadTextures(cBasicTextureManager* g_pTextureManager);
void LoadModelIntoVAO(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID);

void LoadStuff(std::vector<cMesh>& vec_mesh, GLuint& shader_program_ID, cBasicTextureManager* g_pTextureManager);

class cJSONUtility
{
public:

	
	static rapidjson::Document open_document(std::string& filename);
	

};
