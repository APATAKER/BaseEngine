#pragma once
#include "Physics\Physics.h"
#include "GameObject/cGameObject.h"


enum class formationType {
	circle,
	square,
	v,
	line,
	rows
};


class cCoordinator {

public:
    

	void setFormationType(formationType fType)
	{
		this->fType = fType;
	}
	

	void update(double dt,cPhysics* pPhysics,cGameObject* anchorPoint, std::vector<cGameObject*> objects)
	{
		this->getPositions(anchorPoint);

		for (int i = 0; i != 12; i++)
		{
			pPhysics->seek(positions[i],objects[i],dt);
		}
	}

private:

	glm::vec3 positions[12];

	formationType fType;

	void getPositions(cGameObject* anchorPoint)
	{

		if (fType == formationType::circle)
		{
			positions[0] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 0.0f);
			positions[1] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 0.0f);
			positions[2] = glm::normalize(anchorPoint->velocity) + glm::vec3(0.0f, 0.0f, 20.0f);
			positions[3] = glm::normalize(anchorPoint->velocity) + glm::vec3(0.0f, 0.0f, -20.0f);
			positions[4] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, -17.3205f);
			positions[5] = glm::normalize(anchorPoint->velocity) + glm::vec3(17.3205f, 0.0f, -10.0f);
			positions[6] = glm::normalize(anchorPoint->velocity) + glm::vec3(17.3205f, 0.0f, 10.0f);
			positions[7] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 17.3205f);
			positions[8] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 17.3205f);
			positions[9] = glm::normalize(anchorPoint->velocity) + glm::vec3(-17.3205f, 0.0f, 10.0f);
			positions[10] = glm::normalize(anchorPoint->velocity) + glm::vec3(-17.3205f, 0.0f, -10.0f);
			positions[11] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, -17.3205f);
		}

		else if (fType == formationType::v)
		{
			positions[0] = glm::normalize(anchorPoint->velocity);
			positions[1] = glm::normalize(anchorPoint->velocity) + glm::vec3(5.0f, 0.0f, 5.0f);
			positions[2] = glm::normalize(anchorPoint->velocity) + glm::vec3(-5.0f, 0.0f, 5.0f);
			positions[3] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 10.0f);
			positions[4] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 10.0f);
			positions[5] = glm::normalize(anchorPoint->velocity) + glm::vec3(-15.0f, 0.0f, 15.0f);
			positions[6] = glm::normalize(anchorPoint->velocity) + glm::vec3(15.0f, 0.0f, 15.0f);
			positions[7] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 20.0f);
			positions[8] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 20.0f);
			positions[9] = glm::normalize(anchorPoint->velocity) + glm::vec3(-25.0f, 0.0f, 25.0f);
			positions[10] = glm::normalize(anchorPoint->velocity) + glm::vec3(25.0f, 0.0f, 25.0f);
			positions[11] = glm::normalize(anchorPoint->velocity) + glm::vec3(-30.0f, 0.0f, 30.0f);
		}

		else if (fType == formationType::square)
		{
			positions[0] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 0.0f);
			positions[1] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 0.0f);
			positions[2] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 0.0f);
			positions[3] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 0.0f);
			positions[4] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 10.0f);
			positions[5] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 10.0f);
			positions[6] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 10.0f);
			positions[7] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 10.0f);
			positions[8] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, -10.0f);
			positions[9] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, -10.0f);
			positions[10] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, -10.0f);
			positions[11] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, -10.0f);
		}
		else if (fType == formationType::line)
		{
			positions[0] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 0.0f);
			positions[1] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 0.0f);
			positions[2] = glm::normalize(anchorPoint->velocity) + glm::vec3(-30.0f, 0.0f, 0.0f);
			positions[3] = glm::normalize(anchorPoint->velocity) + glm::vec3(-40.0f, 0.0f, 0.0f);
			positions[4] = glm::normalize(anchorPoint->velocity) + glm::vec3(-50.0f, 0.0f, 10.0f);
			positions[5] = glm::normalize(anchorPoint->velocity) + glm::vec3(-60.0f, 0.0f, 10.0f);
			positions[6] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 0.0f);
			positions[7] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 0.0f);
			positions[8] = glm::normalize(anchorPoint->velocity) + glm::vec3(30.0f, 0.0f, 0.0f);
			positions[9] = glm::normalize(anchorPoint->velocity) + glm::vec3(40.0f, 0.0f, 0.0f);
			positions[10] = glm::normalize(anchorPoint->velocity) + glm::vec3(50.0f, 0.0f, 0.0f);
			positions[11] = glm::normalize(anchorPoint->velocity) + glm::vec3(60.0f, 0.0f, 0.0f);
		}

		else
		{
			positions[0] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 0.0f);
			positions[1] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 0.0f);
			positions[2] = glm::normalize(anchorPoint->velocity) + glm::vec3(-30.0f, 0.0f, 0.0f);
			positions[3] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 0.0f);
			positions[4] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 0.0f);
			positions[5] = glm::normalize(anchorPoint->velocity) + glm::vec3(30.0f, 0.0f, 0.0f);
			positions[6] = glm::normalize(anchorPoint->velocity) + glm::vec3(-10.0f, 0.0f, 10.0f);
			positions[7] = glm::normalize(anchorPoint->velocity) + glm::vec3(-20.0f, 0.0f, 10.0f);
			positions[8] = glm::normalize(anchorPoint->velocity) + glm::vec3(-30.0f, 0.0f, 10.0f);
			positions[9] = glm::normalize(anchorPoint->velocity) + glm::vec3(10.0f, 0.0f, 10.0f);
			positions[10] = glm::normalize(anchorPoint->velocity) + glm::vec3(20.0f, 0.0f, 10.0f);
			positions[11] = glm::normalize(anchorPoint->velocity) + glm::vec3(30.0f, 0.0f, 10.0f);
		}
	}
};

