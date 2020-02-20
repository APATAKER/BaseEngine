#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

class iCamera
{
public:
	virtual ~iCamera() {};

	virtual glm::vec3 getEye(void) = 0;
	virtual void setEye(glm::vec3 newEye) = 0;

	virtual glm::vec3 getAt(void) = 0;
	virtual void setAt(glm::vec3 newAt) = 0;

	virtual glm::vec3 getUp(void) = 0;
	virtual void setUp(glm::vec3 newUp) = 0;

	// Used to set various data from the camera
	virtual glm::vec3 getData(std::string command) = 0;

	// Used to set various commands on the camera
	virtual bool Update( std::string command, float data ) = 0;
	virtual bool Update( std::string command, glm::vec2 data ) = 0;
	virtual bool Update( std::string command, glm::vec3 data ) = 0;
	// For cameras that need multiple updates per frame
	virtual bool Update( double deltaTime ) = 0;

};