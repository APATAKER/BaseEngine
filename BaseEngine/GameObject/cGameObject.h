#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>		// The quaternion class
#include <string>

// NOTE: We are including the INTERFACE, not the actual renderer
#include "../DebugRenderer/iDebugRenderer.h"
#include "../ModelLoading/cMesh.h"
#include "../physWrapper/eShapeType.h"
//#include "cPhysComponent.h"
#include "physics/iPhysInterfaces.h"



class cGameObject
{
public:
	cGameObject();
	~cGameObject();
	// Used to draw this mesh
	std::string meshName;			
	unsigned int friendlyIDNumber;
	std::string friendlyName;
	glm::vec3 m_position;
	glm::quat m_qRotation;					// Orientation as a quaternion
	glm::vec3 m_up;
	glm::vec3 m_at;
	glm::vec3 intialFront;
	glm::vec3 initalUp;
	float scale;;   
	glm::vec4  objectColourRGBA;	
	// Default will set to 1.0f
	float alphaTransparency;
	glm::vec4 diffuseColour;		// Colour of the object
	glm::vec4 specularColour;		// Colour of the HIGHLIGHT + Shininess
	                                // RGB - colour
	                                // 4th value (a or w) is the "power"
	                                // 1.0 to 10,000.0f
	enum eObjectType
	{
		PLAYER,
		ENEMY,
		OTHER,
		BULLETP,
		BULLETE// Everything else
	};
	eObjectType objectType;
	cMesh GameObjectMesh;
	glm::mat4 world_transform;
	bool is_static;
	
public:
	glm::quat m_qRotationalVelocity;		// change over time in rotation
	glm::quat getQOrientation(void);
	// updates rotation 
	void Update(double deltaTime);
	void updateAtFromOrientation(void);
	glm::vec3 getCurrentDirection(void);
	glm::vec3 getAtInWorldSpace(void);
	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);

	// Overwrite the orientation
	void setOrientation(glm::vec3 EulerAngleDegreesXYZ);
	void setOrientation(glm::quat qAngle);
	// Updates the existing angle
	void updateOrientation(glm::vec3 EulerAngleDegreesXYZ);
	void updateOrientation(glm::quat qAngle);
	glm::vec3 getEulerAngle(void);

	



	

	// Physics things are in here
	nPhysics::iPhysicsComponent* m_physics_component;
	glm::mat4 DoRender(); 

	//Ai physics stuff
	float collision_radius;
	bool bulletFired;
	bool isDead;

	


	// We could contain another class or struct with the phsyics stuff
	// or we could use this type to determine what we need to check.
	eShapeTypes physicsShapeType;
	glm::vec3 AABB_min, AABB_max;
	float SPHERE_radius;


	// these are the points I'll use for colidiing
	std::vector<glm::vec3> vecPhysTestPoints;

	// Move it based on the "Front" of the object. 
	// ASSUMPTION is the "FRONT" is +ve Z. 
	// LEFT is +ve Z
	// UP is +vs Y
	// NOTE: This will depend on the orientation of your model (in mesh lab)
	void MoveInRelativeDirection(glm::vec3 relativeDirection);


	
	// Won't be lit, and is wireframe
	bool isWireframe;
	glm::vec4 debugColour;
	bool isVisible;
	bool disableDepthBufferTest;
	bool disableDepthBufferWrite;

	// Texture data
	static const int NUMBEROFTEXTURES = 4;
	std::string textures[NUMBEROFTEXTURES];
	float textureRatio[NUMBEROFTEXTURES];

	// Add child object to the this mesh
	std::vector<cGameObject*> vec_pChildObjects;


	unsigned int getUniqueID(void);
	void setDebugRenderer(iDebugRenderer* pDebugRenderer);
	std::string generateStateString(void);
	void parseStateString(std::string saveString);

private:
	// this variable is static, so common to all objects.
	// When the object is created, the unique ID is set, and 
	//	the next unique ID is incremented
	static unsigned int next_uniqueID;
	unsigned int m_uniqueID;

	iDebugRenderer* m_pDebugRenderer;
};

#endif


