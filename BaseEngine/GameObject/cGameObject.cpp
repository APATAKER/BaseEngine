#include "cGameObject.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>



cGameObject::cGameObject()
{
	this->objectType = cGameObject::OTHER;
	
	this->scale = 1.0f;
	this->isVisible = true;

	this->isWireframe = false;
	this->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


	this->objectType = OTHER;
	
	// Set the unique ID
	// Take the value of the static int, 
	//  set this to the instance variable
	this->m_uniqueID = cGameObject::next_uniqueID;
	// Then increment the static variable
	cGameObject::next_uniqueID++;

	this->disableDepthBufferTest = false;
	this->disableDepthBufferWrite = false;

	this->m_pDebugRenderer = NULL;

	this->m_position = glm::vec3(0.0f,0.0f,0.0f);

	// Rotation of 0 degrees.
	this->m_qRotation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->m_qRotationalVelocity = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->intialFront = glm::vec3(0, 0, 1);
	this->initalUp = glm::vec3(0, 1, 0);
	this->m_up = this->initalUp;
	
	this->textures[0] = "defaultTex.bmp";

	this->textureRatio[0] = 1.0f;
	this->textureRatio[1] = 0.0f;
	this->textureRatio[2] = 0.0f;
	this->textureRatio[3] = 0.0f;

	this->alphaTransparency = 1.0f;

	this->bulletFired = false;
	this->isDead = false;
	this->world_transform = glm::mat4(1.f);
	this->is_static = true;

	return;
}

cGameObject::~cGameObject()
{
	//delete this->m_physics_component;
}

// For now, this only updates the rotation over time.
void cGameObject::Update(double deltaTime)
{
	//this->m_qRotationalVelocity;

	//this->m_qRotation += this->m_qRotationalVelocity * (float)deltaTime;
	//this->positionXYZ += this->velocity * (float)deltaTime;
	//this->positionXYZ += glm::lerp( glm::vec3(0.0f,0.0f,0.0f), this->velocity, (float)deltaTime );

	glm::quat qChangeThisFrame = glm::slerp( glm::quat( glm::vec3(0.0f,0.0f,90.0f) ),			// no rotation
						                     this->m_qRotationalVelocity,		// Full rotation IN ONE SECOND
				                             (float)deltaTime);	

	this->m_qRotation *= qChangeThisFrame;

	return;
}


unsigned int cGameObject::getUniqueID(void)
{
	return this->m_uniqueID;
}

void cGameObject::setDebugRenderer(iDebugRenderer* pDebugRenderer)
{
	this->m_pDebugRenderer = pDebugRenderer;
	return;
}


// this variable is static, so common to all objects.
// When the object is created, the unique ID is set, and 
//	the next unique ID is incremented
//static 
unsigned int cGameObject::next_uniqueID = 1000;	// Starting at 1000, just because





void cGameObject::updateAtFromOrientation(void)
{
	glm::mat4 matRotation = glm::mat4(this->m_qRotation);

	glm::vec4 frontOfCamera = glm::vec4(this->intialFront, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->m_at = glm::vec3(newAt);


	return;
}

glm::vec3 cGameObject::getCurrentDirection(void)
{
	this->updateAtFromOrientation();
	
	return this->m_at;
}

glm::vec3 cGameObject::getAtInWorldSpace(void)
{
	//return this->m_physics_component->getPosition() + this->m_at;

	return glm::vec3(0); // bad return TODO
}

void cGameObject::MoveForward_Z(float amount)
{
	//glm::vec3 direction = this->getAtInWorldSpace() - this->m_physics_component->getPosition();

	//// Make direction a "unit length"
	//direction = glm::normalize(direction);

	//// Generate a "forward" adjustment value 
	//glm::vec3 amountToMove = direction * amount;

	//// Add this to the eye
	//this->m_physics_component->setPosition(this->m_physics_component->getPosition()+(amountToMove));

	return;
}

void cGameObject::MoveLeftRight_X(float amount)
{

	/*glm::vec3 vecLeft = glm::cross(this->getCurrentDirection(), this->m_up);

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->m_physics_component->setPosition(this->m_physics_component->getPosition()+(vecAmountToMove));*/
}

//glm::quat m_qRotation;		// Orientation as a quaternion
glm::quat cGameObject::getQOrientation(void)
{
	return this->m_qRotation;
}

// Overwrite the orientation
void cGameObject::setOrientation(glm::vec3 EulerAngleDegreesXYZ)
{
	// c'tor of the glm quaternion converts Euler 
	//	to quaternion representation. 
	glm::vec3 EulerAngleRadians;
	EulerAngleRadians.x = glm::radians(EulerAngleDegreesXYZ.x);
	EulerAngleRadians.y = glm::radians(EulerAngleDegreesXYZ.y);
	EulerAngleRadians.z = glm::radians(EulerAngleDegreesXYZ.z);

	this->m_qRotation = glm::quat(EulerAngleRadians);
}

void cGameObject::setOrientation(glm::quat qAngle)
{
	this->m_qRotation = qAngle;
}

// Updates the existing angle
void cGameObject::updateOrientation(glm::vec3 EulerAngleDegreesXYZ)
{
	// Create a quaternion of this angle...
	glm::vec3 EulerAngleRadians;
	EulerAngleRadians.x = glm::radians(EulerAngleDegreesXYZ.x);
	EulerAngleRadians.y = glm::radians(EulerAngleDegreesXYZ.y);
	EulerAngleRadians.z = glm::radians(EulerAngleDegreesXYZ.z);

	glm::quat angleChange = glm::quat(EulerAngleRadians);

	// ...apply it to the exiting rotation
	this->m_qRotation *= angleChange;
}

void cGameObject::updateOrientation(glm::quat qAngleChange)
{
	this->m_qRotation *= qAngleChange;
}

glm::vec3 cGameObject::getEulerAngle(void)
{
	// In glm::gtx (a bunch of helpful things there)
	glm::vec3 EulerAngle = glm::eulerAngles(this->m_qRotation);

	return EulerAngle;
}

glm::mat4 cGameObject::DoRender()
{

	glm::mat4 worldMatrix(1.f);

	this->m_physics_component->GetTransform(worldMatrix);
	
	return worldMatrix;

	// now use this world matrix to render all the graphics stuff related to this game object.

}


// Move it based on the "Front" of the object. 
// ASSUMPTION is the "FRONT" is +ve Z. 
// LEFT is +ve Z
// UP is +vs Y
// NOTE: This will depend on the orientation of your model (in mesh lab)
void cGameObject::MoveInRelativeDirection(glm::vec3 relativeDirection)
{
	//// The "forward" vector is +ve Z
	//// (the 4th number is because we need a vec4 later)
	//glm::vec4 forwardDirObject = glm::vec4( relativeDirection, 1.0f );

	//glm::mat4 matModel = glm::mat4(1.0f);	// Identity

	//// Roation
	//// Constructor for the GLM mat4x4 can take a quaternion
	//glm::mat4 matRotation = glm::mat4(this->getQOrientation());
	//matModel *= matRotation;
	//// *******************

	//// Like in the vertex shader, I mulitly the test points
	//// by the model matrix (MUST be a VEC4 because it's a 4x4 matrix)
	//glm::vec4 forwardInWorldSpace = matModel * forwardDirObject;


	//// Add this to the position of the object
	////this->positionXYZ += glm::vec3( forwardInWorldSpace );
	//this->m_physics_component->setVelocity(this->m_physics_component->getVelocity() + glm::vec3(forwardInWorldSpace));
	////this->velocity += glm::vec3( forwardInWorldSpace );

	return;
}


std::string cGameObject::generateStateString(void)
{
	return std::string();
}

void cGameObject::parseStateString(std::string saveString)
{
	// TODO:

	return;
}
