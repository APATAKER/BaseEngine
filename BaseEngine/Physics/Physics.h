#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include "../GameObject/cGameObject.h"
#include "../ModelLoading/cMesh.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct  State
{
	glm::vec3 pos;
	glm::vec3 vel;
};
struct Derivative
{
	glm::vec3 dx;
	glm::vec3 dv;
};





class cPhysics
{
public:
	cPhysics();

	struct wanderDetails
	{
		wanderDetails(glm::vec3 wanderPt,float radius);
		glm::vec3 wanderPt;
		float radius;
	};
	
	// Alias to a type "existing type" "new type name"
	typedef glm::vec3 Point;
	typedef glm::vec3 Vector;

	float slowingRadius = 10.0f;
	float maxVelocity = 50.0f;

	struct Sphere
	{
		glm::vec3 c;
		float r;
	};

	struct sPhysicsTriangle
	{
		glm::vec3 verts[3];
		glm::vec3 normal;
	};

	
	Derivative evaluate(const State& initial, glm::vec3 gravity,
		float dt,
		const Derivative& d);


	glm::vec3 acceleration(const State& state, double t);
	

	void integrate(std::vector<cGameObject*>& vec_pGameObjects, float dt);

	// Bullet stuff

	void bulletHolder(std::vector<cGameObject*>& vec_pGameObjects);

	void bulletShoot(std::vector<cGameObject*>& vec_pGameObjects);
	void bulletShoot(cGameObject* enemyobj);
	int DeadEnemyCalu(std::vector<cGameObject*>& vec_pGameObjects);

	void respawnEnemy(std::vector<cGameObject*>& vec_pGameObjects);
	
	


	struct sCollisionInfo
	{
		glm::vec3 closestPoint;
		glm::vec3 directionOfApproach;
		float penetrationDistance;
		glm::vec3 adjustmentVector;
		unsigned int Object1_ID;		// Shpere
		unsigned int Object2_ID;		// Sphere or Triangle
	};

	template <class T>
	T randInRange(T min, T max)
	{
		double value =
			min + static_cast <double> (rand())
			/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
		return static_cast<T>(value);
	};

	// This "moves" the objects based on the inegration step
	void IntegrationStep(std::vector<cGameObject*>& vec_pGameObjects,
		float deltaTime);
	/*void IntegrationStep(std::vector<cPhysicsObject*>& vec_pPhysicsObject,
		float deltaTime);*/

	void TestForCollisions(std::vector<cGameObject*>& vec_pGameObjects);

	// Returns all the triangles and the closest points
	void GetClosestTriangleToPoint(Point pointXYZ, cMesh& mesh, glm::vec3& closestPoint, sPhysicsTriangle& closestTriangle);

	// Will return the closest triangles that are within the range "distanceRange".
	// This can be used as a "closest triangles to sphere"
	void GetClosestTrianglesToSphere(cGameObject& testSphere, float distanceRange, cMesh& mesh, std::vector<sPhysicsTriangle>& vecClosestTriangles);

	void CheckIfCrossedEndBound(std::vector<cGameObject*>& vec_pGameObjects);
	// Taken from Ericson's book:
	Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c);
	int TestSphereTriangle(Sphere s, Point a, Point b, Point c, Point& p);

	void setGravity(glm::vec3 newGravityValue);
	glm::vec3 getGravity(void);

	// Takes a mesh in "model space" and converts it into "world space"
	void CalculateTransformedMesh(cMesh& originalMesh, glm::mat4 matWorld,
		cMesh& mesh_transformedInWorld);


	//AI functions
	void seek(cGameObject* target, cGameObject* aiObj, double deltatime);
	void seek(glm::vec3 targetPos, cGameObject* aiObj, double deltatime);

	void pursue(cGameObject* target, cGameObject* aiObj, double deltatime);

	void evade(cGameObject* target, cGameObject* aiObj, double deltatime);
	
	
	void wander(int& flag,std::vector<wanderDetails> wanderPts, cGameObject* aiObj, double deltatime);

	void flee(cGameObject* target, cGameObject* aiObj, double deltatime);

	void checkBulletCollision(std::vector<cGameObject*>& vec_pGameObjects);

	glm::quat safeQuatLookAt(
		glm::vec3 const& lookFrom,
		glm::vec3 const& lookTo,
		glm::vec3 const& up);

	void aiMotion1(cGameObject* target, cGameObject* aiObj, double deltatime);
	void aiMotion2(cGameObject* target, cGameObject* aiObj, double deltatime);
	void aiMotion3(cGameObject* target, cGameObject* aiObj, double deltatime);

	


private:

	// Does collision test and returns collision information
	// Returns true if collision, and will load collisionInfo struct
	bool DoSphereSphereCollisionTest(cGameObject* pA, cGameObject* pB,
		sCollisionInfo& collisionInfo);
	bool DoShphereMeshCollisionTest(cGameObject* pA, cGameObject* pB,
		sCollisionInfo& collisionInfo);


	glm::vec3  m_Gravity;

};

#endif
