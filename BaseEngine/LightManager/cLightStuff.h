#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <vector>

namespace mLight
{
	void LoadLightFromJson();

	
	class cLightStuff
	{
	public:
		cLightStuff();
		~cLightStuff();

		friend void LoadLightFromJson();
		void loadLightIntoShader(GLuint shader_program_id, int num_of_lights);
	
	private:
		int light_id;
		glm::vec3 light_position;
		float light_const_atten;
		float light_linear_atten;
		float light_quad_atten;
		float light_spot_inner_angle;
		float light_spot_outer_angle;
		glm::vec3 light_spot_direction;
		
	};
	

}
