#pragma once
#include "glm_headers.h"

// nCollide
//
// A namespace to isolate functions taken directly from
// REAL-TIME COLLISION DETECTION, ERICSON
//
// The functions have been slightly altered to:
// a) Return additional information about shapes "already" colliding.
// b) Implement the glm math library
namespace nCollide
{
	// closest_point_on_plane
	// Given a plane and any point in space, determine the closest point.
	// 
	// point : any point in space
	// n : plane normal
	// d : plane dot product
	// returns the closest point on the plane of point projected onto the plane
	glm::vec3 closest_point_on_plane(
		const glm::vec3& point, const glm::vec3& n, float d);
	// c : sphere center
	// r : sphere radius
	// v : sphere movement
	// n : plane normal
	// d : plane dot product
	// t (output) : [0,1] A ratio of the timestep, indicating the time of collision.
	// q (output) : The collision point, where the sphere first contacts the plane.
	// Returns one of three values: [-1,0,1]
	// Returns -1 if a collision was already happening at the beginning of the timestep.
	// Returns  0 if no collision happend during the timestep.
	// Returns  1 if a collision happend during the timestep.
	int intersect_moving_sphere_plane(
		const glm::vec3& c, float r,
		const glm::vec3& v, const glm::vec3& n, float d,
		float& t, glm::vec3& q);
	// TODO: comments!
	int intersect_ray_sphere(
		const glm::vec3& point, const glm::vec3& dir,
		const glm::vec3& center, float radius,
		float& t, glm::vec3& q);
	// TODO: comments!
	int intersect_moving_sphere_sphere(
		const glm::vec3& c0, float r0, const glm::vec3& v0,
		const glm::vec3& c1, float r1, const glm::vec3& v1, float& t);
}