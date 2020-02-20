#include "cLightStuff.h"
#include "../JsonLoader/cLoad.h"

#include <string>
extern rapidjson::Document document;
extern std::vector<mLight::cLightStuff*> vec_lightObjects;


mLight::cLightStuff::cLightStuff()
	:light_position(glm::vec3(-25.0f, 300.0f, -150.0f))
	,light_const_atten(0.0000001f)
	,light_linear_atten(0.00119f)
	,light_quad_atten(9.21e-7f)
	,light_spot_inner_angle(5.0f)
	,light_spot_outer_angle(7.5f)
	,light_spot_direction(glm::vec3(0.0f, -1.0f, 0.0f))
{
}

mLight::cLightStuff::~cLightStuff()
{
}

void mLight::cLightStuff::loadLightIntoShader(GLuint shader_program_ID,int num_of_lights)
{
	
	
	for(int index = 0;index<num_of_lights;index++)
	{	
		std::string prefix = "theLights["+ std::to_string(index)+"].";
		std::string pos_light = prefix + "position";
		GLint L_position = glGetUniformLocation(shader_program_ID, pos_light.c_str());
		std::string dis_light = prefix + "diffuse";
		GLint L_diffuse = glGetUniformLocation(shader_program_ID, dis_light.c_str());
		std::string spec_light = prefix + "specular";
		GLint L_specular = glGetUniformLocation(shader_program_ID, spec_light.c_str());
		std::string atten_light = prefix + "atten";
		GLint L_atten = glGetUniformLocation(shader_program_ID, atten_light.c_str());
		std::string dirc_light = prefix + "direction";
		GLint L_direction = glGetUniformLocation(shader_program_ID, dirc_light.c_str());
		std::string par1_light = prefix + "param1";
		GLint L_param1 = glGetUniformLocation(shader_program_ID, par1_light.c_str());
		std::string par2_light = prefix + "param2";
		GLint L_param2 = glGetUniformLocation(shader_program_ID, par2_light.c_str());
		
		glUniform4f(L_position,
			vec_lightObjects[index]->light_position.x,
			vec_lightObjects[index]->light_position.y,
			vec_lightObjects[index]->light_position.z,
			1.0f);
		glUniform4f(L_diffuse, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_specular, 1.0f, 1.0f, 1.0f, 1.0f);	// White
		glUniform4f(L_atten, 0.0f,  // constant attenuation
			vec_lightObjects[index]->light_linear_atten,  // Linear 
			vec_lightObjects[index]->light_quad_atten,	// Quadratic 
			1000000.0f);	// Distance cut off
		// Point light:
		glUniform4f(L_param1, 0.0f /*POINT light*/, 0.0f, 0.0f, 1.0f);
		glUniform4f(L_param2, 1.0f /*Light is on*/, 0.0f, 0.0f, 1.0f);
	}
}

	//lights into shader
	//GLint L_0_position = glGetUniformLocation(shader_program_ID, "theLights[0].position");
	//GLint L_0_diffuse = glGetUniformLocation(shader_program_ID, "theLights[0].diffuse");
	//GLint L_0_specular = glGetUniformLocation(shader_program_ID, "theLights[0].specular");
	//GLint L_0_atten = glGetUniformLocation(shader_program_ID, "theLights[0].atten");
	//GLint L_0_direction = glGetUniformLocation(shader_program_ID, "theLights[0].direction");
	//GLint L_0_param1 = glGetUniformLocation(shader_program_ID, "theLights[0].param1");
	//GLint L_0_param2 = glGetUniformLocation(shader_program_ID, "theLights[0].param2");

void mLight::LoadLightFromJson()
{
	if(document["Lights"].IsArray())
		for (int i = 0; i < document["Lights"].Size(); i++)
		{
			rapidjson::Value& jlightobj = document["Lights"][i];
			cLightStuff* light_obj = new cLightStuff();
			light_obj->light_id = jlightobj["lightID"].GetInt();
			light_obj->light_position = glm::vec3(jlightobj["lightPosition"]["x"].GetFloat()
												, jlightobj["lightPosition"]["y"].GetFloat()
												, jlightobj["lightPosition"]["z"].GetFloat());
			light_obj->light_const_atten = jlightobj["lightConstAtten"].GetFloat();
			light_obj->light_linear_atten = jlightobj["lightLinearAtten"].GetFloat();
			light_obj->light_quad_atten = jlightobj["lightQuadAtten"].GetFloat();
			light_obj->light_spot_inner_angle = jlightobj["lightInnerAngle"].GetFloat();
			light_obj->light_spot_outer_angle = jlightobj["lightOuterAngle"].GetFloat();
			light_obj->light_spot_direction= glm::vec3(jlightobj["lightDirection"]["x"].GetFloat()
													 , jlightobj["lightDirection"]["y"].GetFloat()
													 , jlightobj["lightDirection"]["z"].GetFloat());

			vec_lightObjects.push_back(light_obj);
		}
}
