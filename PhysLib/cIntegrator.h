#pragma once
#include "glm_headers.h"

namespace physLib
{
	// cIntegrator
	// 
	// A single place for numerical integration schemes.
	//
	// Contains single entry-points for each supported integrations scheme.
	class cIntegrator
	{
	private:
		struct State
		{
			glm::vec3 pos;
			glm::vec3 vel;
		};
		struct Derivative
		{
			glm::vec3 dx;            // dx/dt = velocity
			glm::vec3 dv;			 // dv/dt = accleration
		};
		struct sRK4State;
		

		//function used in RK4 calculations
		Derivative evaluate(const State& initial, glm::vec3 acc, float dt, const Derivative& d);

	public:
		// Constructor
		cIntegrator();

		// EULER ALGORITHM
		void Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt); // also have this
		void Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// EULER-CROMER ALGORITHM
		void EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// MIDPOINT ALGORITHM
		void MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// VELOCITY-VERLET ALGORITHM <- with half-step acceleration lag
		void VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
		// FOURTH-ORDER RUNGE-KUTTA
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);   // doing this
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);

		
	};
}