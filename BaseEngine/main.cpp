#include "commonHeaders.h"												// common headers
#include "ProjectStuff/openGLStuff.h"									// userInput and create OpenGL window
#include "ModelLoadingAndVAO/cVAOManager.h"								// VAO loader from model
#include "DebugRenderer/cDebugRenderer.h"								// DebugRenderer
#include "FlyCamera/cFlyCamera.h"										// Camera
#include "DeltaTime/cLowPassFilter.h"									// DeltaTime calcu
#include "JsonLoader/cLoad.h"											// Json Loader
#include "global.h"														// Global Loading AND func in future
#include "FBO/cFBO.h"
#include "MazeGen/cMazeMaker.h"
#include "LightManager/cLightStuff.h"
#include "Graph/Graph.h"

// Global Pointers and variables
GLFWwindow* window = nullptr;
mLight::cLightStuff* p_light_stuff = nullptr;
cBasicTextureManager* g_pTextureManager = nullptr;
cFBO* p_fbo1 = nullptr;
cFBO* p_fbo2 = nullptr;
cDebugRenderer* g_pDebugRenderer = nullptr;
cFlyCamera* g_pFlyCamera = nullptr;
//cLowPassFilter* avgDeltaTimeThingy = nullptr;
cLowPassFilter* p_low_pass_filter = nullptr;
cMazeMaker* p_maze_maker = nullptr;
double deltaTime = 0;

cVAOManager* p_vao_manager = cVAOManager::getInstance();   // Singleton Here
GLuint g_shader_program_ID;

std::vector<cGameObject*> g_vec_pGameObjects;
std::vector<mLight::cLightStuff*> vec_lightObjects;
std::vector<cGameObject*> vec_bullets;
std::vector<cMesh> vec_model_mesh;

glm::vec3 g_HACK_vec3_BoneLocationFK = glm::vec3(0.0f);
extern int punchcounter;
extern int currentLight;

// Global Functions
void DrawObject(glm::mat4 matWorld,cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager);
glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject, glm::mat4 matWorld);
void SetUpTextureBindingsForObject(
	cGameObject* pCurrentObject,
	GLint shaderProgID);
cMesh findMeshByName(std::vector<cMesh> vMesh, std::string Meshname);
cGameObject* findGameObjectByFriendlyName(std::vector<cGameObject*> vGameObjects, std::string friendlyname);


int main()
{
	Graph* graph = new Graph();

	graph->CreateNode('a');
	
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
	::g_pFlyCamera->eye = glm::vec3(-2.5,175,323.9);
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

	p_maze_maker = new cMazeMaker();
	int maze_width =  20;
	int maze_height = 20;
	p_maze_maker->GenerateMaze(maze_width, maze_height);

	g_pFlyCamera->setAt(-g_pFlyCamera->getAt());

	p_light_stuff = new mLight::cLightStuff();
	mLight::LoadLightFromJson();

	//int numberOfStencilBits = 0;
	//glGetFramebufferAttachmentParameteriv(
	//	GL_FRAMEBUFFER,
	//	GL_STENCIL,
	//	GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &numberOfStencilBits);
	//std::cout << "Stencil buffer is " << numberOfStencilBits << " bits" << std::endl;

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
			<< g_pFlyCamera->eye.x << ", "
			<< g_pFlyCamera->eye.y << ", "
			<< g_pFlyCamera->eye.z
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
		PhysicsInit();
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
		//

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

		glUniform1i(passNumber_UniLoc, 2);
		cGameObject* p_TV_screen1 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen1");
		glm::mat4 mat4_TV_screen1 = glm::mat4(1.f);
		DrawObject(mat4_TV_screen1,p_TV_screen1, g_shader_program_ID, p_vao_manager);
		
		glUniform1i(passNumber_UniLoc, 3);
		cGameObject* p_TV_screen2 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen2");
		glm::mat4 mat4_TV_screen2 = glm::mat4(1.f);
		DrawObject(mat4_TV_screen2,p_TV_screen2, g_shader_program_ID, p_vao_manager);

		glUniform1i(passNumber_UniLoc, 4);
		cGameObject* p_TV_screen3 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen3");
		glm::mat4 mat4_TV_screen3 = glm::mat4(1.f);
		DrawObject(mat4_TV_screen3,p_TV_screen3, g_shader_program_ID, p_vao_manager);

		glUniform1i(passNumber_UniLoc, 5);
		cGameObject* p_TV_screen4 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen4");
		glm::mat4 mat4_TV_screen4 = glm::mat4(1.f);
		DrawObject(mat4_TV_screen4,p_TV_screen4, g_shader_program_ID, p_vao_manager);
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
		
		
										
		
		
		// 4. Draw the TV and Screen
		glUniform1i(SelectEffect_UL, 0);
		glUniform1i(passNumber_UniLoc, 6);
		//cGameObject* quad = findGameObjectByFriendlyName(g_vec_pGameObjects, "QUAD");
		glm::mat4 matWorld = glm::mat4(1.f);
		DrawObject(matWorld, QUAD, g_shader_program_ID, p_vao_manager);
		//// GameObject Draw Call
		//for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		//{
		//	cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];
		//	glm::mat4 matWorld = glm::mat4(1.f);
		//	if (pCurrentObject->m_physics_component)
		//	{
		//		int number_of_physical_objects = pCurrentObject->m_physics_component->GetNumberOfPhysicalObject();
		//		for (int i = 0; i < number_of_physical_objects; i++)
		//		{
		//			glm::mat4 matWorldPhy = glm::mat4(1.f);
		//			pCurrentObject->m_physics_component->GetTransform(i, matWorldPhy);
		//			DrawObject(matWorldPhy, pCurrentObject,
		//				g_shader_program_ID, p_vao_manager);
		//		}
		//	}
		//	else
		//	{
		//		if ((pCurrentObject->friendlyName) != "tvscreen1"
		//			&& (pCurrentObject->friendlyName) != "tvscreen2"
		//			&& (pCurrentObject->friendlyName) != "tvscreen3"
		//			&& (pCurrentObject->friendlyName) != "tvscreen4")
		//			DrawObject(matWorld, pCurrentObject, g_shader_program_ID, p_vao_manager);
		//	}

		//}//for (int index...
		//{
		//	////cGameObject* debug_sphere = findGameObjectByFriendlyName(g_vec_pGameObjects, "debugsphere");
		//	//debug_sphere->isVisible = true;
		//	//debug_sphere->m_position = g_HACK_vec3_BoneLocationFK;
		//	//glm::mat4 identmat = glm::mat4(1.0f);
		//	//DrawObject(identmat, debug_sphere, g_shader_program_ID, p_vao_manager);
		//	//debug_sphere->isVisible = false;
		//	////debug_sphere->m_position = debug_sphere_old;	
		//}
		//
		////glUniform1i(passNumber_UniLoc, 2);
		////cGameObject* p_TV_screen1 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen1");
		////glm::mat4 mat4_TV_screen1 = glm::mat4(1.f);
		////DrawObject(mat4_TV_screen1,p_TV_screen1, g_shader_program_ID, p_vao_manager);
		////
		////glUniform1i(passNumber_UniLoc, 3);
		////cGameObject* p_TV_screen2 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen2");
		////glm::mat4 mat4_TV_screen2 = glm::mat4(1.f);
		////DrawObject(mat4_TV_screen2,p_TV_screen2, g_shader_program_ID, p_vao_manager);

		////glUniform1i(passNumber_UniLoc, 4);
		////cGameObject* p_TV_screen3 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen3");
		////glm::mat4 mat4_TV_screen3 = glm::mat4(1.f);
		////DrawObject(mat4_TV_screen3,p_TV_screen3, g_shader_program_ID, p_vao_manager);

		////glUniform1i(passNumber_UniLoc, 5);
		////cGameObject* p_TV_screen4 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen4");
		////glm::mat4 mat4_TV_screen4 = glm::mat4(1.f);
		////DrawObject(mat4_TV_screen4,p_TV_screen4, g_shader_program_ID, p_vao_manager);

		//glUniform1i(passNumber_UniLoc, 0);
				
		//Physics implementation
		
		PhysicsUpdate(deltaTime);

		//Physics implementation

		glfwSwapBuffers(window);		// Buffer Swap
		glfwPollEvents();

	}
	//############################## Game Loop Ends Here ##################################################################




	// Destroy 
	glfwDestroyWindow(window);
	glfwTerminate();

	
	delete p_vao_manager;
	delete g_pFlyCamera;
	delete g_pDebugRenderer;
	delete g_pTextureManager;
	delete p_maze_maker;
	delete p_light_stuff;
	delete p_low_pass_filter;
	/*delete pPhysics;*/
	PhysicsEnd();
	for (int i = 0; i < g_vec_pGameObjects.size(); i++)
		delete g_vec_pGameObjects[i];
	exit(EXIT_SUCCESS);

	return 0;
}

void DrawObject(glm::mat4 matWorld,cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager)
{
	// Uniforms in shaders
	GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgID, "bIsSkyBox");
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");
	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");
	GLint isSkinnedMesh_UniLoc = glad_glGetUniformLocation(shaderProgID, "isSkinnedMesh");
	GLint matBonesArray_UniLoc = glGetUniformLocation(shaderProgID, "matBonesArray");
	// TO Draw Or Not
	if (pCurrentObject->isVisible == false)
	{
		return;
	}
	// Turns on "alpha transparency"
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ************ 
	// Set the texture bindings and samplers
	// See if this is a skybox object? 
	if (pCurrentObject->friendlyName != "skybox")
	{
		// Is a regular 2D textured object
		SetUpTextureBindingsForObject(pCurrentObject, shaderProgID);			// Binding textures for normal object (4 Textures[4])
		glUniform1f(bIsSkyBox_UL, (float)GL_FALSE);
		// Don't draw back facing triangles (default)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);		// Don't draw "back facing" triangles
	}
	else
	{
		// Draw the back facing triangles. 
		// Because we are inside the object, so it will force a draw on the "back" of the sphere 
		//glCullFace(GL_FRONT_AND_BACK);
		glDisable(GL_CULL_FACE);	// Draw everything
		glUniform1f(bIsSkyBox_UL, (float)GL_TRUE);
		GLuint skyBoxTextureID = ::g_pTextureManager->getTextureIDFromName("space");
		glActiveTexture(GL_TEXTURE0 +10);				// Texture Unit 10
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);	// Texture now assoc with texture unit 0
		// Tie the texture units to the samplers in the shader
		GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgID, "skyBox");		// Binding SKYBOX TEXTURES
		glUniform1i(skyBoxSampler_UL, 10);	// Texture unit 10
	}
	// ************	
	// Calculating the Transforms (T/R/S)
	glm::mat4 matWorldCurrentGO = calculateWorldMatrix(pCurrentObject, matWorld);
	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));

	// Setting the Diffuse Color
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->alphaTransparency);	

	// Setting the Specular Color
	glUniform4f(specularColour_UL,
		1.0f,	// R
		1.0f,	// G
		1.0f,	// B
		1000.0f);	// Specular "power" (how shinny the object is)
					// 1.0 to really big (10000.0f)

	// Is WireFrame or Not
	if (pCurrentObject->isWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// LINES
		glUniform4f(debugColour_UL,
			pCurrentObject->debugColour.r,
			pCurrentObject->debugColour.g,
			pCurrentObject->debugColour.b,
			pCurrentObject->debugColour.a);
		glUniform1f(bDoNotLight_UL, (float)GL_TRUE);
	}
	else
	{	// Regular object (lit and not wireframe)
		glUniform1f(bDoNotLight_UL, (float)GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// SOLID
	}

	if (pCurrentObject->disableDepthBufferTest)
	{
		glDisable(GL_DEPTH_TEST);					// DEPTH Test OFF
	}
	else
	{
		glEnable(GL_DEPTH_TEST);						// Turn ON depth test
	}

	if (pCurrentObject->disableDepthBufferWrite)
	{
		glDisable(GL_DEPTH);						// DON'T Write to depth buffer
	}
	else
	{
		glEnable(GL_DEPTH);								// Write to depth buffer
	}

	// Performing Animations
	if (pCurrentObject->p_skinned_mesh != NULL)
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_TRUE);
		std::string animationToPlay = "";
		float curFrameTime = 0.0;
		cAnimationState* pAniState = pCurrentObject->pAniState;
		// Are there any animations in the queue of animations
		if (!pAniState->vecAnimationQueue.empty())
		{
			// Play the "1st" animation in the queue 
			animationToPlay = pAniState->vecAnimationQueue[0].name;
			curFrameTime = pAniState->vecAnimationQueue[0].currentTime;
			// Increment the top animation in the queue
			if (pAniState->vecAnimationQueue[0].IncrementTime())
			{ 
				if(pAniState->vecAnimationQueue.begin()->name == "punchright" || pAniState->vecAnimationQueue.begin()->name == "punchleft")
				{
					punchcounter = 0;
				}
				pAniState->vecAnimationQueue.erase(pAniState->vecAnimationQueue.begin());

			}//vecAnimationQueue[0].IncrementTime()
		}
		else
		{	// Use the default animation.
			pAniState->defaultAnimation.IncrementTime();
			animationToPlay = pAniState->defaultAnimation.name;
			curFrameTime = pAniState->defaultAnimation.currentTime;

		}//if ( pAniState->vecAnimationQueue.empty()
		// Taken from "Skinned Mesh 2
		std::vector< glm::mat4x4 > vecFinalTransformation;
		std::vector< glm::mat4x4 > vecOffsets;
		std::vector< glm::mat4x4 > vecObjectBoneTransformation;
		
		// This loads the bone transforms from the animation model
		pCurrentObject->p_skinned_mesh->BoneTransform(curFrameTime,	
			animationToPlay,
			vecFinalTransformation,
			vecObjectBoneTransformation,
			vecOffsets);
		
		// Forward kinematic stuff
		{	
			// "Bone" location is at the origin
			glm::vec4 boneLocation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glm::mat4 matSpecificBone = vecObjectBoneTransformation[22];
			// bone #22 is "B_R_Hand" in this model
			// Transformed into "model" space where that bone is.
			::g_HACK_vec3_BoneLocationFK = matSpecificBone * boneLocation;	
			//			// If it's in world space
			//			::g_HACK_vec3_BoneLocationFK = matModel * ::g_HACK_vec3_BoneLocationFK;
		}
		// Forward kinematic 
		GLint numBonesUsed = (GLint)vecFinalTransformation.size();
		glUniformMatrix4fv(matBonesArray_UniLoc, numBonesUsed,
			GL_FALSE,
			glm::value_ptr(vecFinalTransformation[0]));
	}
	else
	{
		glUniform1f(isSkinnedMesh_UniLoc, (float)GL_FALSE);
	}
	// ************************************************
	
	sModelDrawInfo drawInfo;
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElementsBaseVertex(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0,
			0);
		glBindVertexArray(0);
	}
	return;
} // DrawObject;

void SetUpTextureBindingsForObject(
	cGameObject* pCurrentObject,
	GLint shaderProgID)
{
	// Tie the texture to the texture unit
	for(int i=0;i<4;i++)
	{
		GLuint texSamp_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->textures[i]);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texSamp_UL);

		std::string texsample = "textSamp0"+std::to_string(i);
		//std::cout << texsample << std::endl;
		GLint textSamp_UL = glGetUniformLocation(shaderProgID, texsample.c_str());
		glUniform1i(textSamp_UL, i);
	}	
	GLint tex0_ratio_UL = glGetUniformLocation(shaderProgID, "tex_0_3_ratio");
	glUniform4f(tex0_ratio_UL,
		pCurrentObject->textureRatio[0],		// 1.0
		pCurrentObject->textureRatio[1],
		pCurrentObject->textureRatio[2],
		pCurrentObject->textureRatio[3]);
	return;
}

glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject, glm::mat4 matWorld)
{
	if (!pCurrentObject->m_physics_component)
	{
		glm::mat4 matTrans = glm::translate(glm::mat4(1.f), pCurrentObject->m_position);
		matWorld = matWorld * matTrans;

		glm::mat4 matRotation = glm::mat4(pCurrentObject->getQOrientation());
		matWorld = matWorld * matRotation;
	}
	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->scale,
			pCurrentObject->scale,
			pCurrentObject->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********
	return matWorld;
}

cMesh findMeshByName(std::vector<cMesh> vMesh,std::string Meshname)
{
	for(int i=0;i<vMesh.size();i++)
	{
		if (vMesh[i].meshname == Meshname)
			return  vMesh[i];
	}
}

cGameObject* findGameObjectByFriendlyName(std::vector<cGameObject*> vGameObjects,std::string friendlyname)
{
	for(int i =0;i<vGameObjects.size();i++)
	{
		if (vGameObjects[i]->friendlyName == friendlyname)
			return vGameObjects[i];
	}
}
