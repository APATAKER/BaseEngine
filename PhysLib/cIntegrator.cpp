#include "cIntegrator.h"  // My header

namespace physLib
{

	// Reused Function for RK4 implementation
	cIntegrator::Derivative cIntegrator::evaluate(const State& initial, glm::vec3 acc, float dt, const Derivative& d)
	{
		State state;
		state.pos = initial.pos + d.dx * dt;
		state.vel = initial.vel + d.dv * dt;
		Derivative output;
		output.dx = state.vel;
		output.dv = acc;
		return output;
	}
	cIntegrator::cIntegrator()
	{
	}

	// EULER ALGORITHM
	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		vel += acc * dt;
		pos += vel * dt;
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
		Derivative a, b, c, d;

		State state;
		state.pos = pos;
		state.vel = vel;
		a = evaluate(state, acc, 0.0f, Derivative());
		b = evaluate(state, acc, dt * 0.5f, a);
		c = evaluate(state, acc, dt * 0.5f, b);
		d = evaluate(state, acc, dt, c);

		float dxdtX = 1.0f / 6.0f *
			(a.dx.x + 2.0f * (b.dx.x + c.dx.x) + d.dx.x);
		float dxdtY = 1.0f / 6.0f *
			(a.dx.y + 2.0f * (b.dx.y + c.dx.y) + d.dx.y);
		float dxdtZ = 1.0f / 6.0f *
			(a.dx.z + 2.0f * (b.dx.z + c.dx.z) + d.dx.z);

		float dvdtX = 1.0f / 6.0f *
			(a.dv.x + 2.0f * (b.dv.x + c.dv.x) + d.dv.x);
		float dvdtY = 1.0f / 6.0f *
			(a.dv.y + 2.0f * (b.dv.y + c.dv.y) + d.dv.y);
		float dvdtZ = 1.0f / 6.0f *
			(a.dv.z + 2.0f * (b.dv.z + c.dv.z) + d.dv.z);

		state.pos.x = state.pos.x + dxdtX * dt;
		state.pos.y = state.pos.y + dxdtY * dt;
		state.pos.z = state.pos.z + dxdtZ * dt;
		
		state.vel.x = state.vel.x + dvdtX * dt;
		state.vel.y = state.vel.y + dvdtY * dt;
		state.vel.z = state.vel.z + dvdtZ * dt;

		pos = state.pos;
		vel = state.vel;
	}

	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
		// For an excellent article on RK4 implementation:
		// https://gafferongames.com/post/integration_basics/
	}
}