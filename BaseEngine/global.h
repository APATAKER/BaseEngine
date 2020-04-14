#pragma once
#include "commonHeaders.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "DeltaTime/cLowPassFilter.h"
#include "FBO/cFBO.h"
#include "FiniteStateMachine/FSMSystem.h"
#include "FlyCamera/cFlyCamera.h"
#include "LightManager/cLightStuff.h"
#include "MapLoader/BMPImage.h"
#include "Textures/cBasicTextureManager.h"

class cVAOManager;
inline GLFWwindow* window = nullptr;
inline mLight::cLightStuff* p_light_stuff = nullptr;
inline cBasicTextureManager* g_pTextureManager = nullptr;
inline cFBO* p_fbo1 = nullptr;
inline cFBO* p_fbo2 = nullptr;
inline cDebugRenderer* g_pDebugRenderer = nullptr;
inline cFlyCamera* g_pFlyCamera = nullptr;
inline cLowPassFilter* p_low_pass_filter = nullptr;
inline BMPImage* p_map_from_bmp = nullptr;
inline FSMSystem* p_fsm_system = nullptr;

inline double deltaTime = 0;

inline std::vector<cGameObject*> g_vec_pGameObjects;
inline std::vector<mLight::cLightStuff*> vec_lightObjects;
inline std::vector<cGameObject*> vec_bullets;
inline std::vector<cMesh> vec_model_mesh;


void PhysicsInit();
void PhysicsEnd();
void PhysicsUpdate(float dt);

void DrawObject(glm::mat4 matWorld, cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager);
	
