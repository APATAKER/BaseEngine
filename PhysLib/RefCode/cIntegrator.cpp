#include "cIntegrator.h"  // My header

namespace phys
{
	cIntegrator::cIntegrator()
	{

	}

	// EULER ALGORITHM
	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// EULER-CROMER ALGORITHM
	void cIntegrator::EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::EulerCromer(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// MIDPOINT ALGORITHM
	void cIntegrator::MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::MidPoint(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// VELOCITY-VERLET ALGORITHM <- with half-step acceleration lag
	void cIntegrator::VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
	}

	void cIntegrator::VelocityVerlet(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
	}

	// FOURTH-ORDER RUNGE-KUTTA
	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
		// For an excellent article on RK4 implementation:
		// https://gafferongames.com/post/integration_basics/
	}

	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
		// For an excellent article on RK4 implementation:
		// https://gafferongames.com/post/integration_basics/
	}
}