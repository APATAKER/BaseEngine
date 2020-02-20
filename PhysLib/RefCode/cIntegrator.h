#pragma once
#include <game_math.h>   // single include header for all glm math stuffs

namespace phys
{
	// cIntegrator
	// 
	// A single place for numerical integration schemes.
	//
	// Contains single entry-points for each supported integrations scheme.
	class cIntegrator
	{
	private:
		// TODO: Define any structs used internally for specific schemes.
		//       Such as,
		struct State;
		struct Derivative;
		struct sRK4State;
		// TODO: Define any methods used interally for specific schemes.

	public:
		// Constructor
		cIntegrator();

		// TODO: Do you really want all these options? Do you want others?
		//       You really only need RK4 (or Velocity Verlet) for Project 2.

		// TODO: Fully comment the inputs/outputs of any
		//       integration method you do end up including.

		// EULER ALGORITHM
		void Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
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
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
	};
}