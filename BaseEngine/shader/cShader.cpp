#include "cShaderManager.h"
#include "../commonHeaders.h"		// glfw.h, etc. (openGL.h)

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderManager::cShader::~cShader() 
{
	return;
}

std::string cShaderManager::cShader::getShaderTypeString(void)
{
	switch ( this->shaderType )
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	// Should never reach here...
	return "UNKNOWN_SHADER_TYPE";
}


//// TODO: For the students to do, because it's FUN, FUN, FUN
//std::map< std::string /*name of uniform variable*/,
//		    unsigned int /* uniform location ID */ > 
//						mapUniformName_to_UniformLocation;

// Look up the uniform inside the shader, then save it, if it finds it
bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string variableName)
{
	// 
	GLint uniLocation = glGetUniformLocation(this->ID, 
											 variableName.c_str() );
	// Did it find it (not -1)
	if ( uniLocation == -1 )
	{	// Nope.
		return false;
	}
	// Save it
	this->mapUniformName_to_UniformLocation[variableName.c_str()] = uniLocation;

	return true;	
}

// Look up the uniform location and save it.
int cShaderManager::cShaderProgram::getUniformID_From_Name(std::string name)
{
	std::map< std::string /*name of uniform variable*/,
			  int /* uniform location ID */ >::iterator 
		itUniform = this->mapUniformName_to_UniformLocation.find(name);

	if ( itUniform == this->mapUniformName_to_UniformLocation.end() )
	{
		return -1;		// OpenGL uniform not found value
	}

	return itUniform->second;		// second if the "int" value
}


void cShaderManager::cShaderProgram::LoadActiveUniforms(void)
{
	// Find out the max size of the buffer we need. 
	int maxNameSize = 0;
	glGetProgramiv(this->ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameSize);
	std::cout << "Max uniform name size is : " << maxNameSize << std::endl;

	// Create a buffer of this max size
	char* uniformName = new char[maxNameSize];
	// Clear it
	memset(uniformName, 0, maxNameSize);

	// How many uniforms? 
	int numberOfActiveUniforms = 0;
	glGetProgramiv(this->ID, GL_ACTIVE_UNIFORMS, &numberOfActiveUniforms);
	std::cout << "There are " << numberOfActiveUniforms << " active uniforms" << std::endl;

	int sexy = 0;

	for (int index = 0; index != numberOfActiveUniforms; index++)
	{
		const int BUFFERSIZE = 1000;
		char myBuffer[BUFFERSIZE] = { 0 };

		GLsizei numCharWritten = 0;
		GLint sizeOfVariable = 0;
		GLenum uniformType = 0;
		memset(uniformName, 0, maxNameSize);


		GLenum hey = GL_FLOAT_VEC2;

		glGetActiveUniform(this->ID,
			index,				// Uniform index (0 to ...)
			BUFFERSIZE,			// Max number of chars
			&numCharWritten,	// How many it REALLY wrote
			&sizeOfVariable,	// Size in bytes?
			&uniformType,
			uniformName);

		std::cout << "Uniform # " << index << std::endl;
		std::cout << "\t" << uniformName << std::endl;
		std::cout << "\t" << getNameStringFromType(uniformType) << std::endl;
		std::cout << "\t" << sizeOfVariable << std::endl;

		sexy = 0;

	}//for (int index = 0

	return;
}

std::string cShaderManager::cShaderProgram::getNameStringFromType(int glVariableType)
{
	// from : https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml

	switch (glVariableType)
	{
	case GL_FLOAT: return "GL_FLOAT"; break;
	case GL_FLOAT_VEC2: return "GL_FLOAT_VEC2"; break;
	case GL_FLOAT_VEC3: return "GL_FLOAT_VEC3"; break;
	case GL_FLOAT_VEC4: return "GL_FLOAT_VEC4"; break;
	case GL_DOUBLE: return "GL_DOUBLE"; break;
	case GL_DOUBLE_VEC2: return "GL_DOUBLE_VEC2"; break;
	case GL_DOUBLE_VEC3: return "GL_DOUBLE_VEC3"; break;
	case GL_DOUBLE_VEC4: return "GL_DOUBLE_VEC4"; break;
	case GL_INT: return "GL_INT"; break;

		// And so on... 

	default:
		return "I HAVE NO IDEA!!!";
	}

	return "I HAVE NO IDEA!!!";
}



int cShaderManager::cShaderProgram::getUniformLocID(std::string uniformname)
{
	std::map<std::string /*uniform name*/,
		GLint /*uniform location*/>::iterator itUniform
		= map_UniformNameToLocID.find(uniformname);

	if (itUniform != map_UniformNameToLocID.end())
	{
		return itUniform->second;
	}

	// Didn't find it.
	return -1;
}