#include "global.h"
#include "Windows.h"
#include "physics/iPhysInterfaces.h"
#include <iostream>
#include "JsonLoader/cLoad.h"


#include "GameObject/cGameObject.h"


nPhysics::iPhysicsFactory* physics_factory = nullptr;
nPhysics::iPhysicsWorld* physics_world = nullptr;
typedef nPhysics::iPhysicsFactory* (*func_createFactory)();
HMODULE hModule = 0;
func_createFactory create_factory = 0;
extern std::vector<cGameObject*> g_vec_pGameObjects;
bool changePhys(true);
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
					g_vec_pGameObjects[i]->m_physics_component = physics_factory->CreatePlane(plane_def);
				}
				break;
			}
			physics_world->AddComponent(g_vec_pGameObjects[i]->m_physics_component);
		}
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
