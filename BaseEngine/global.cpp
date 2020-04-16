#include "global.h"

#include "physics/iPhysInterfaces.h"
#include "JsonLoader/cLoad.h"


#include "GameObject/cGameObject.h"

extern int punchcounter;
nPhysics::iPhysicsFactory* physics_factory = nullptr;
nPhysics::iPhysicsWorld* physics_world = nullptr;
typedef nPhysics::iPhysicsFactory* (*func_createFactory)();
HMODULE hModule = 0;
func_createFactory create_factory = 0;
extern std::vector<cGameObject*> g_vec_pGameObjects;
bool changePhys(false);
extern rapidjson::Document document;
int dataLoaded;


void PhysicsInit()
{
	//DLL loading
	if(dataLoaded == 0)
	{
	if(changePhys == true)
	{
		hModule = LoadLibrary(L"BulletPhysicsWrapper.dll");
	}
	else
	{
		hModule = LoadLibrary(L"PhysWrapper.dll");	
	}
	if (!hModule)
	{
		std::cout << "failed to load the library" << std::endl;
		exit(1);
	}
	create_factory = (func_createFactory)GetProcAddress(hModule, "make_physics_factory");
	if (!create_factory)
	{
		std::cout << "failed to get the physics Factory function" << std::endl;
		exit(1);
	}
	physics_factory = create_factory();
	physics_world = physics_factory->CreateWorld();

	
			
		for(int i=0;i< g_vec_pGameObjects.size();i++)
		{
			if(!g_vec_pGameObjects[i]->isWireframe)
			{
				
			rapidjson::Value& jgameobj = document["GameObjects"][i];
			switch (g_vec_pGameObjects[i]->physicsShapeType)
			{
			case eShapeTypes::SPHERE:
			{
				nPhysics::sBallDef ball_def;
				ball_def.Position = glm::vec3(jgameobj["position"]["x"].GetFloat(),
					jgameobj["position"]["y"].GetFloat(),
					jgameobj["position"]["z"].GetFloat());
				ball_def.Mass = jgameobj["mass"].GetFloat();
				ball_def.Radius = jgameobj["sphere"]["radius"].GetFloat();
				ball_def.orientation = g_vec_pGameObjects[i]->getQOrientation();
				ball_def.AiType = jgameobj["aiType"].GetString();
				g_vec_pGameObjects[i]->m_physics_component = physics_factory->CreateBall(ball_def);

			}
			break;
			case eShapeTypes::PLANE:
			{
				nPhysics::sPlaneDef plane_def;
				plane_def.position = glm::vec3(jgameobj["position"]["x"].GetFloat(),
					jgameobj["position"]["y"].GetFloat(),
					jgameobj["position"]["z"].GetFloat());
				plane_def.Normal = glm::vec3(jgameobj["plane"]["normal"]["x"].GetFloat(),
					jgameobj["plane"]["normal"]["y"].GetFloat(),
					jgameobj["plane"]["normal"]["z"].GetFloat());
				plane_def.Constant = jgameobj["plane"]["constant"].GetFloat();
				plane_def.AiType = jgameobj["aiType"].GetString();
				plane_def.orientation = g_vec_pGameObjects[i]->getQOrientation();
				g_vec_pGameObjects[i]->m_physics_component = physics_factory->CreatePlane(plane_def);
			}
			break;
			case eShapeTypes::CLOTH:
			{
				nPhysics::sClothDef cloth_def;
				cloth_def.CornerA = glm::vec3(jgameobj["cloth"]["CornerA"]["x"].GetFloat(),
											  jgameobj["cloth"]["CornerA"]["y"].GetFloat(),
											  jgameobj["cloth"]["CornerA"]["z"].GetFloat());
				cloth_def.CornerB = glm::vec3(jgameobj["cloth"]["CornerB"]["x"].GetFloat(),
											  jgameobj["cloth"]["CornerB"]["y"].GetFloat(),
											  jgameobj["cloth"]["CornerB"]["z"].GetFloat());
				cloth_def.DownDirection = glm::vec3(jgameobj["cloth"]["DownDirection"]["x"].GetFloat(),
													jgameobj["cloth"]["DownDirection"]["y"].GetFloat(),
													jgameobj["cloth"]["DownDirection"]["z"].GetFloat());
				cloth_def.NumNodesAcross = jgameobj["cloth"]["NumNodesAcross"].GetInt();
				cloth_def.NumNodesDown = jgameobj["cloth"]["NumNodesDown"].GetInt();
				cloth_def.NodeMass = jgameobj["cloth"]["NodeMass"].GetFloat();
				cloth_def.SpringConstant = jgameobj["cloth"]["SpringConstant"].GetFloat();
				g_vec_pGameObjects[i]->m_physics_component = physics_factory->CreateCloth(cloth_def);


			}
			break;
			}
			physics_world->AddComponent(g_vec_pGameObjects[i]->m_physics_component);
			}
		}
		physics_world->SetUpAi();
		dataLoaded = 1;
	}
	
}

void PhysicsEnd()
{
	delete physics_world;
	delete physics_factory;
	FreeLibrary(hModule);
}

void PhysicsUpdate(float dt)
{
	physics_world->Update(dt);
}


void DrawObject(glm::mat4 matWorld, cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager)
{
	// Uniforms in shaders
	GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgID, "bIsSkyBox");
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");
	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");
	GLint isSkinnedMesh_UniLoc = glad_glGetUniformLocation(shaderProgID, "isSkinnedMesh");
	GLint matBonesArray_UniLoc = glGetUniformLocation(shaderProgID, "matBonesArray");
	// TO Draw Or Not
	if (pCurrentObject->isVisible == false)
	{
		return;
	}
	// Turns on "alpha transparency"
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ************ 
	// Set the texture bindings and samplers
	// See if this is a skybox object? 
	if (pCurrentObject->friendlyName != "skybox")
	{
		// Is a regular 2D textured object
		SetUpTextureBindingsForObject(pCurrentObject, shaderProgID);			// Binding textures for normal object (4 Textures[4])
		glUniform1f(bIsSkyBox_UL, (float)GL_FALSE);
		// Don't draw back facing triangles (default)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);		// Don't draw "back facing" triangles
	}
	else
	{
		// Draw the back facing triangles. 
		// Because we are inside the object, so it will force a draw on the "back" of the sphere 
		//glCullFace(GL_FRONT_AND_BACK);
		glDisable(GL_CULL_FACE);	// Draw everything
		glUniform1f(bIsSkyBox_UL, (float)GL_TRUE);
		GLuint skyBoxTextureID = ::g_pTextureManager->getTextureIDFromName("space");
		glActiveTexture(GL_TEXTURE0 + 10);				// Texture Unit 10
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);	// Texture now assoc with texture unit 0
		// Tie the texture units to the samplers in the shader
		GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgID, "skyBox");		// Binding SKYBOX TEXTURES
		glUniform1i(skyBoxSampler_UL, 10);	// Texture unit 10
	}
	// ************	
	// Calculating the Transforms (T/R/S)
	glm::mat4 matWorldCurrentGO = calculateWorldMatrix(pCurrentObject, matWorld);
	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

	// Setting the Diffuse Color
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->alphaTransparency);

	// Setting the Specular Color
	glUniform4f(specularColour_UL,
		1.0f,	// R
		1.0f,	// G
		1.0f,	// B
		1000.0f);	// Specular "power" (how shinny the object is)
					// 1.0 to really big (10000.0f)

	// Is WireFrame or Not
	if (pCurrentObject->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			pCurrentObject->debugColour.r,
			pCurrentObject->debugColour.g,
			pCurrentObject->debugColour.b,
			pCurrentObject->debugColour.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}

	if (pCurrentObject->disableDepthBufferTest)
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
	}

	if (pCurrentObject->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_DEPTH);								// Write to depth buffer
	}

	// Performing Animations
	if (pCurrentObject->p_skinned_mesh != NULL)
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_TRUE);
		std::string animationToPlay = "";
		float curFrameTime = 0.0;
		cAnimationState* pAniState = pCurrentObject->pAniState;
		// Are there any animations in the queue of animations
		if (!pAniState->vecAnimationQueue.empty())
		{
			// Play the "1st" animation in the queue 
			animationToPlay = pAniState->vecAnimationQueue[0].name;
			curFrameTime = pAniState->vecAnimationQueue[0].currentTime;
			// Increment the top animation in the queue
			if (pAniState->vecAnimationQueue[0].IncrementTime())
			{
				if (pAniState->vecAnimationQueue.begin()->name == "punchright" || pAniState->vecAnimationQueue.begin()->name == "punchleft")
				{
					punchcounter = 0;
				}
				pAniState->vecAnimationQueue.erase(pAniState->vecAnimationQueue.begin());

			}//vecAnimationQueue[0].IncrementTime()
		}
		else
		{	// Use the default animation.
			pAniState->defaultAnimation.IncrementTime();
			animationToPlay = pAniState->defaultAnimation.name;
			curFrameTime = pAniState->defaultAnimation.currentTime;

		}//if ( pAniState->vecAnimationQueue.empty()
		// Taken from "Skinned Mesh 2
		std::vector< glm::mat4x4 > vecFinalTransformation;
		std::vector< glm::mat4x4 > vecOffsets;
		std::vector< glm::mat4x4 > vecObjectBoneTransformation;

		// This loads the bone transforms from the animation model
		pCurrentObject->p_skinned_mesh->BoneTransform(curFrameTime,
			animationToPlay,
			vecFinalTransformation,
			vecObjectBoneTransformation,
			vecOffsets);

		// Forward kinematic stuff
		{
			// "Bone" location is at the origin
			glm::vec4 boneLocation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glm::mat4 matSpecificBone = vecObjectBoneTransformation[22];
			// bone #22 is "B_R_Hand" in this model
			// Transformed into "model" space where that bone is.
			//::g_HACK_vec3_BoneLocationFK = matSpecificBone * boneLocation;
			//			// If it's in world space
			//			::g_HACK_vec3_BoneLocationFK = matModel * ::g_HACK_vec3_BoneLocationFK;
		}
		// Forward kinematic 
		GLint numBonesUsed = (GLint)vecFinalTransformation.size();
		glUniformMatrix4fv(matBonesArray_UniLoc, numBonesUsed,
			GL_FALSE,
			glm::value_ptr(vecFinalTransformation[0]));
	}
	else
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_FALSE);
	}
	// ************************************************

	sModelDrawInfo drawInfo;
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElementsBaseVertex(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0,
			0);
		glBindVertexArray(0);
	}
	return;
} // DrawObject;


void SetUpTextureBindingsForObject(
	cGameObject* pCurrentObject,
	GLint shaderProgID)
{
	// Tie the texture to the texture unit
	for (int i = 0; i < 4; i++)
	{
		GLuint texSamp_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->textures[i]);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texSamp_UL);

		std::string texsample = "textSamp0" + std::to_string(i);
		//std::cout << texsample << std::endl;
		GLint textSamp_UL = glGetUniformLocation(shaderProgID, texsample.c_str());
		glUniform1i(textSamp_UL, i);
	}
	GLint tex0_ratio_UL = glGetUniformLocation(shaderProgID, "tex_0_3_ratio");
	glUniform4f(tex0_ratio_UL,
		pCurrentObject->textureRatio[0],		// 1.0
		pCurrentObject->textureRatio[1],
		pCurrentObject->textureRatio[2],
		pCurrentObject->textureRatio[3]);
	return;
}

glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject, glm::mat4 matWorld)
{
	if (!pCurrentObject->m_physics_component)
	{
		glm::mat4 matTrans = glm::translate(glm::mat4(1.f), pCurrentObject->m_position);
		matWorld = matWorld * matTrans;

		glm::mat4 matRotation = glm::mat4(pCurrentObject->getQOrientation());
		matWorld = matWorld * matRotation;
	}
	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->scale,
			pCurrentObject->scale,
			pCurrentObject->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********
	return matWorld;
}


cMesh findMeshByName(std::vector<cMesh> vMesh, std::string Meshname)
{
	for (int i = 0; i < vMesh.size(); i++)
	{
		if (vMesh[i].meshname == Meshname)
			return  vMesh[i];
	}
}

cGameObject* findGameObjectByFriendlyName(std::vector<cGameObject*> vGameObjects, std::string friendlyname)
{
	for (int i = 0; i < vGameObjects.size(); i++)
	{
		if (vGameObjects[i]->friendlyName == friendlyname)
			return vGameObjects[i];
	}
}

char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255 && g == 0 && b == 0)		return 'r';
	if (r == 0 && g == 255 && b == 0)		return 'g';
	if (r == 0 && g == 0 && b == 255)	return 'b';
	if (r == 255 && g == 255 && b == 255)	return 'w';
	if (r == 0 && g == 0 && b == 0)		return '_';
	return 'x';
}


