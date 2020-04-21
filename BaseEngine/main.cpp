#include "commonHeaders.h"												// common headers
#include "ProjectStuff/openGLStuff.h"									// userInput and create OpenGL window
#include "ModelLoadingAndVAO/cVAOManager.h"								// VAO loader from model
#include "DebugRenderer/cDebugRenderer.h"								// DebugRenderer
#include "FlyCamera/cFlyCamera.h"										// Camera
#include "DeltaTime/cLowPassFilter.h"									// DeltaTime calcu
#include "JsonLoader/cLoad.h"											// Json Loader
#include "global.h"
#include "MapLoader/ResourceManager.h"
#include "FiniteStateMachine/States.h"

// Global Pointers and variables
cVAOManager* p_vao_manager = cVAOManager::getInstance();   // Singleton Here
GLuint g_shader_program_ID;
extern int currentLight;

// Global Functions is Not at Global header file
int main()
{
	// opengl call
	window = creatOpenGL(window);

	// Debug Renderer
	cDebugRenderer* g_pDebugRenderer = new cDebugRenderer();
	if (!g_pDebugRenderer->initialize())
	{
		std::cout << "Error init on DebugShader: " << g_pDebugRenderer->getLastError() << std::endl;
	}

	//########################################## Json is loader Here ###############################################
	InitJson("Config/config.json");
	// Loading mesh model, shaders, drawinfo for shader and textures and gameobjects
	LoadStuff(vec_model_mesh, g_shader_program_ID, g_pTextureManager,g_vec_pGameObjects);
	g_pTextureManager = cBasicTextureManager::getInstance();
	//########################################## Json is loader Here ###############################################
	

	//##### GAME ### OBJECTS ### TO ### CREATED ### HERE ##################(ONLY FOR DEBUG AND MULTIPLE)#############
	cGameObject* debug_sphere = new cGameObject();
	debug_sphere->meshName = "sphere5";
	debug_sphere->scale = 5.f;
	debug_sphere->isWireframe = true;
	debug_sphere->isVisible = false;
	debug_sphere->physicsShapeType = eShapeTypes::STATIC;

	cGameObject* QUAD = new cGameObject();
	QUAD->meshName = "QUAD";
	QUAD->m_position = glm::vec3(0, 0, 0);
	QUAD->scale = 1.f;
	QUAD->isVisible = 1.f;
	QUAD->physicsShapeType = eShapeTypes::STATIC;
	//##### GAME ### OBJECTS ### TO ### CREATED ### HERE ############################################################

	// Camera Created here
	::g_pFlyCamera = new cFlyCamera();
	::g_pFlyCamera->eye = glm::vec3(-34.632,493.674,1606.22);
	::g_pFlyCamera->movementSpeed = 0.25f;
	::g_pFlyCamera->movementSpeed = 2.5f;
	// Camera Created here

	
	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	
	//############################### Calculating DeltaTime #################################
	p_low_pass_filter = new cLowPassFilter();
	p_low_pass_filter->startTime();
	//############################### Calculating DeltaTime #################################

	
	// Set up the FBO object
	p_fbo1 = new cFBO();
	p_fbo2 = new cFBO();
	// Usually we make this the size of the screen.
	std::string FBOError;
	if (p_fbo1->init(1920, 1080, FBOError))
	{
		std::cout << "Frame buffer 1 is OK" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOError << std::endl;
	}
	if (p_fbo2->init(1920, 1080, FBOError))
	{
		std::cout << "Frame buffer 2 is OK" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOError << std::endl;
	}


	g_pFlyCamera->setAt(-g_pFlyCamera->getAt());

	p_light_stuff = new mLight::cLightStuff();
	mLight::LoadLightFromJson();
	PhysicsInit();
	//g_vec_pGameObjects[20]->m_physics_component->SetPosition(gatherer_starting_position);

	//############################## Game Loop Starts Here ##################################################################
	while (!glfwWindowShouldClose(window))
	{
		// temp variables
		float ratio;
		int width, height;
		glm::mat4 p, v;
		GLint passNumber_UniLoc = glGetUniformLocation(g_shader_program_ID, "passNumber");
		GLint SelectEffect_UL = glGetUniformLocation(g_shader_program_ID, "selectEffect");
		GLint eyeLocation_UL = glGetUniformLocation(g_shader_program_ID, "eyeLocation");
		GLint matView_UL = glGetUniformLocation(g_shader_program_ID, "matView");
		GLint matProj_UL = glGetUniformLocation(g_shader_program_ID, "matProj");
		GLint screenWidth_UnitLoc = glGetUniformLocation(g_shader_program_ID, "screenWidth");
		GLint screenHeight_UnitLoc = glGetUniformLocation(g_shader_program_ID, "screenHeight");
		std::stringstream ssTitle;
		// temp variables

		// Window title Printing (top of the window)
		ssTitle
			<< g_vec_pGameObjects[4]->m_position.x << ", "
			<< g_vec_pGameObjects[4]->m_position.y << ", "
			<< g_vec_pGameObjects[4]->m_position.z
			/*<< g_pFlyCamera->eye.x << ", "
			<< g_pFlyCamera->eye.y << ", "
			<< g_pFlyCamera->eye.z*/
			<< "Light No: "
			<< vec_lightObjects[currentLight]->light_id<<" "
			<< "Light postion: "
			<< vec_lightObjects[currentLight]->light_position.x<<" "
			<< vec_lightObjects[currentLight]->light_position.y<<" "
			<< vec_lightObjects[currentLight]->light_position.z
			<< "Light linear atten: "
			<< vec_lightObjects[currentLight]->light_linear_atten<<" "
			<< "Quad atten: "
			<< vec_lightObjects[currentLight]->light_quad_atten<<" "
			<< std::endl;
		glfwSetWindowTitle(window, ssTitle.str().c_str());
		
		// Updating DeltaTime *********************************************
		p_low_pass_filter->updateTime(deltaTime);
		
		// Updating DeltaTime *********************************************

		ProcessAsyncKeys(window);		// Listening to keyboard keys
		ProcessAsyncMouse(window);		// Listening to mouse

		glUseProgram(g_shader_program_ID);		// using current shader

		//lights into shader
		p_light_stuff->loadLightIntoShader(g_shader_program_ID, vec_lightObjects.size());

		//PASS 1 - original COLOR return *****************************
		glBindFramebuffer(GL_FRAMEBUFFER, p_fbo1->ID);
		p_fbo1->clearBuffers(true, true);
		glUniform1i(passNumber_UniLoc, 0);		//Normal Pass
		glUniform1i(SelectEffect_UL, 2);		//Original Color
		// Creating the GL_ViewPort   This is Pass 1 
		glfwGetFramebufferSize(window, &width, &height);
		//std::cout << width << " " << height << std::endl;
		glUniform1f(screenWidth_UnitLoc, width);
		glUniform1f(screenHeight_UnitLoc, height);
		ratio = (float)width / (float)height;
		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			1.0f,			// Near clipping plane
			10000.0f);		// Far clipping plane
		// View matrix
		v = glm::mat4(1.0f);
		v = glm::lookAt(::g_pFlyCamera->getEye(),
			::g_pFlyCamera->getAtInWorldSpace(),
			::g_pFlyCamera->getUpVector());
		//view and projection into shader
		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));
		// camera into shader
		glUniform4f(eyeLocation_UL,
			::g_pFlyCamera->getEye().x,
			::g_pFlyCamera->getEye().y,
			::g_pFlyCamera->getEye().z, 1.0f);

		// GameObject Draw Call
		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		{
			glm::mat4 matWorld = glm::mat4(1.f);
			cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];
			if (pCurrentObject->m_physics_component)
			{
				int number_of_physical_objects = pCurrentObject->m_physics_component->GetNumberOfPhysicalObject();
				for (int i = 0; i < number_of_physical_objects; i++)
				{
					glm::mat4 matWorldPhy = glm::mat4(1.f);
					pCurrentObject->m_physics_component->GetTransform(i, matWorldPhy);
					DrawObject(matWorldPhy, pCurrentObject,
						g_shader_program_ID, p_vao_manager);
				}
			}
			else
			{
				if ((pCurrentObject->friendlyName) != "tvscreen1"
					&& (pCurrentObject->friendlyName) != "tvscreen2"
					&& (pCurrentObject->friendlyName) != "tvscreen3"
					&& (pCurrentObject->friendlyName) != "tvscreen4")
				DrawObject(matWorld, pCurrentObject, g_shader_program_ID, p_vao_manager);
			}
		}//for (int index...
		// 3. Set up the textures for the TV screen (From the FBO)
		glActiveTexture(GL_TEXTURE0 + 40);				// Texture Unit 40
		glBindTexture(GL_TEXTURE_2D, p_fbo1->colourTexture_0_ID);	// Texture now asbsoc with texture unit 40      // Basically binding to
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);													// out vec4 pixelColor
		GLint color_pass_texture_UL = glGetUniformLocation(g_shader_program_ID, "secondPassColourTexture");
		glUniform1i(color_pass_texture_UL, 40);	// Texture unit 40

		glActiveTexture(GL_TEXTURE0 + 41);				// Texture Unit 41
		glBindTexture(GL_TEXTURE_2D, p_fbo1->normalTexture_ID);	// Texture now asbsoc with texture unit 41			// Basically binding to
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);													// out vec4 pixelNormal
		GLint normal_pass_texture_UL = glGetUniformLocation(g_shader_program_ID, "secondPassNormalTexture");
		glUniform1i(normal_pass_texture_UL, 41);	// Texture unit 41

		glActiveTexture(GL_TEXTURE0 + 42);				// Texture Unit 42
		glBindTexture(GL_TEXTURE_2D, p_fbo1->vertWorldPositionTexture_ID);// Texture now asbsoc with texture unit 42 // Basically binding to
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);													 // out vec4 pixelVertWorldPosition
		GLint vertWorldPos_pass_texture_UL = glGetUniformLocation(g_shader_program_ID, "secondPassVertWorldPositionTexture");
		glUniform1i(vertWorldPos_pass_texture_UL, 42);	// Texture unit 42

		glActiveTexture(GL_TEXTURE0 + 43);				// Texture Unit 43
		glBindTexture(GL_TEXTURE_2D, p_fbo1->SpecularTexture_ID);	// Texture now asbsoc with texture unit 43			// Basically binding to
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);														// out vec4 pixelSpecular
		GLint specular_pass_texture_UL = glGetUniformLocation(g_shader_program_ID, "secondPassSpecularTexture");
		glUniform1i(specular_pass_texture_UL, 43);	// Texture unit 43

		//PASS 1 *********************************************************

		// PASS 2 - Depth color return *****************************
		glBindFramebuffer(GL_FRAMEBUFFER, p_fbo2->ID);
		p_fbo2->clearBuffers(true, true);
		glUniform1i(passNumber_UniLoc, 0);				// Normal Pass
		glUniform1i(SelectEffect_UL, 1);				// Depths color
		// GameObject Draw Call
		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		{
			glm::mat4 matWorld = glm::mat4(1.f);
			cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];
			if (pCurrentObject->m_physics_component)
			{
				int number_of_physical_objects = pCurrentObject->m_physics_component->GetNumberOfPhysicalObject();
				for (int i = 0; i < number_of_physical_objects; i++)
				{
					glm::mat4 matWorldPhy = glm::mat4(1.f);
					pCurrentObject->m_physics_component->GetTransform(i, matWorldPhy);
					DrawObject(matWorldPhy, pCurrentObject,
						g_shader_program_ID, p_vao_manager);
				}
			} else{
				DrawObject(matWorld, pCurrentObject, g_shader_program_ID, p_vao_manager);
			}
		}//for (int index...
		glActiveTexture(GL_TEXTURE0 + 44);				// Texture Unit 44
		glBindTexture(GL_TEXTURE_2D, p_fbo2->colourTexture_0_ID);	// Texture now asbsoc with texture unit 44
		GLint depth_pass_texture_UL = glGetUniformLocation(g_shader_program_ID, "secondPassDepthTexture");
		glUniform1i(depth_pass_texture_UL, 44);	// Texture unit 44				
		// PASS 2 *****************************************************

		// PASS 3 // Draw image on screen
		// 1. Set the framebuffer to the Actual screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. Clear the screen (glClear())
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);					// For rendering the scene to fit the current size of the window
		ratio = (float)width / (float)height;
		// Projection matrix
		p = glm::perspective(0.6f,		// FOV
			ratio,			// Aspect ratio
			1.0f,			// Near clipping plane
			10000.0f);		// Far clipping plane
		// View matrix
		v = glm::mat4(1.0f);
		//v = glm::lookAt(::g_pFlyCamera->getEye(),
		//	::g_pFlyCamera->getAtInWorldSpace(),
		//	::g_pFlyCamera->getUpVector());
		v = glm::lookAt(glm::vec3(0,0,2000.f)/*glm::vec3(-45.66f, 115.6f, 1796.14f)*/,
			glm::vec3(0.f,0.f,0.f),
			glm::vec3(0.f,1.f,0.f));
		//view and projection into shader
		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));
		//glUniform4f(eyeLocation_UL,
		//	-45.66f,
		//	115.6f,
		//	1796.14f, 1.0f);
		
		// 4. Draw the Screen on the Quad for deferred Rendering
		glUniform1i(SelectEffect_UL, 0);
		glUniform1i(passNumber_UniLoc, 6);
		//cGameObject* quad = findGameObjectByFriendlyName(g_vec_pGameObjects, "QUAD");
		glm::mat4 matWorld = glm::mat4(1.f);
		DrawObject(matWorld, QUAD, g_shader_program_ID, p_vao_manager);

		
		//Physics implementation
		//p_fsm_system->Update(g_vec_pGameObjects[20]);
		//g_vec_pGameObjects[20]->m_physics_component->ApplyForce(g_vec_pGameObjects[20]->m_velocity);
		PhysicsUpdate(deltaTime);
		//Physics implementation

		glfwSwapBuffers(window);		// Buffer Swap
		glfwPollEvents();

	}
	//############################## Game Loop Ends Here ##################################################################

	// Destroy (Garbage Collection)
	glfwDestroyWindow(window);
	glfwTerminate();

	
	delete p_vao_manager;
	delete g_pFlyCamera;
	delete g_pDebugRenderer;
	delete g_pTextureManager;
	delete p_light_stuff;
	delete p_low_pass_filter;
	PhysicsEnd();
	for (int i = 0; i < g_vec_pGameObjects.size(); i++)
		delete g_vec_pGameObjects[i];
	
	exit(EXIT_SUCCESS);
}
