#include "commonHeaders.h"						// common headers
#include "ProjectStuff/openGLStuff.h"			// userInput and create OpenGL window
#include "ModelLoading/cModelLoader.h"			// PLY model loader and ASSIMP model Loader
#include "shader/cShaderManager.h"				// Shader stuff
#include "VAO/cVAOManager.h"					// VAO loader from model
#include "Textures/cBasicTextureManager.h"		// Texture loading
#include "DebugRenderer/cDebugRenderer.h"		// DebugRenderer
#include "GameObject/cGameObject.h"				// GameObjects
#include "FlyCamera/cFlyCamera.h"				// Camera
#include "DeltaTime/cLowPassFilter.h"			// DeltaTime calcu
#include "JsonLoader/cLoad.h"					// Json Loader
#include <physics/iPhysInterfaces.h>			// Physics
#include "global.h"								// Global Loading AND func in future
#include "FBO/cFBO.h"
#include "MazeGen/cMazeMaker.h"
#include "LightManager/cLightStuff.h"


// Global Pointers and variables
cBasicTextureManager* g_pTextureManager = nullptr;
cMazeMaker* p_maze_maker = nullptr;
cFBO* p_fbo = nullptr;
GLFWwindow* window = nullptr;
cDebugRenderer* g_pDebugRenderer = nullptr;
cFlyCamera* g_pFlyCamera = nullptr;
cLowPassFilter* avgDeltaTimeThingy = nullptr;
mLight::cLightStuff* p_light_stuff = nullptr;
extern nPhysics::iPhysicsFactory* physics_factory;
extern nPhysics::iPhysicsWorld* physics_world;


std::vector<cGameObject*> g_vec_pGameObjects;
std::vector<mLight::cLightStuff*> vec_lightObjects;
std::vector<cGameObject*> vec_bullets;

rapidjson::Document document;




// Global Functions
void DrawObject(glm::mat4 matWorld,
	cGameObject* pCurrentObject,
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
	
	
	
	// opengl call
	window = creatOpenGL(window);

	// Debug Renderer
	cDebugRenderer* g_pDebugRenderer = new cDebugRenderer();
	if (!g_pDebugRenderer->initialize())
	{
		std::cout << "Error init on DebugShader: " << g_pDebugRenderer->getLastError() << std::endl;
	}



	//########################################## Json is loader Here ###############################################
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	std::string jsonFilename = "Config/config.json";
	document = cJSONUtility::open_document(jsonFilename);
	//########################################## Json is loader Here ###############################################


	
	
	///######################## MODEL #### LOADING ##### STARTS ### HERE ##########################################
	cModelLoader* p_model_loader = new cModelLoader();
	
	// Models Loaded here	
	std::string error_string = "";
	size_t num_of_models = document["models"].Size(); // getting number of models
	std::vector<cMesh> vec_model_mesh;
	for (size_t c = 0; c < num_of_models; c++)
	{
		cMesh Mesh;
		if (p_model_loader->LoadModel_Assimp(document["models"][c].GetString(), Mesh, error_string))
		{
			std::cout << document["models"][c].GetString()<< " model loaded" << std::endl;
			Mesh.meshname = document["MeshName"][c].GetString();
			vec_model_mesh.push_back(Mesh);
		}
		else
		{
			std::cout << "\nerror:" << error_string << std::endl;
		}
	}
	///######################## MODEL #### LOADING ##### ENDS ### HERE ##########################################

	
	///######################## SHADER #### LOADING ## STARTS ### HERE #############################################
	cShaderManager* p_shader_manager = new cShaderManager();
	cShaderManager::cShader vertex_shader;
	vertex_shader.fileName = document["shaders"]["vert"].GetString();
	cShaderManager::cShader fragment_shader;
	fragment_shader.fileName = document["shaders"]["frag"].GetString();
	if (!p_shader_manager->createProgramFromFile("SimpleShader", vertex_shader, fragment_shader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << p_shader_manager->getLastError();
		return -1;
	}
	GLuint shader_program_ID = p_shader_manager->getIDFromFriendlyName("SimpleShader");
	///######################## SHADER #### LOADING ## ENDS ### HERE #############################################


	//##### MODELS ### LOADING ### INTO ### VERTEX ### ARRAY ### OBJECT #### (DATA PUSHED INTO SHADER CODE PART)###########
	
	cVAOManager* p_vao_manager = cVAOManager::getInstance();   // Singleton Here
	for (size_t c = 0; c < num_of_models; c++)
	{
		sModelDrawInfo draw_info;
		p_vao_manager->LoadModelIntoVAO(document["MeshName"][c].GetString(), vec_model_mesh[c], draw_info, shader_program_ID);
	}
	//##### MODELS ### LOADING ### INTO ### VERTEX ### ARRAY ### OBJECT #### (DATA PUSHED INTO SHADER CODE PART)###########

	
	//######################################### Loading Textures Here ################################################
	::g_pTextureManager = new cBasicTextureManager();
	::g_pTextureManager->SetBasePath("assets/textures");
	// Normal Textures
	size_t numTex = document["Textures"].Size();
	for(size_t c=0;c<numTex;c++)
	{
		if (!::g_pTextureManager->Create2DTextureFromBMPFile(document["Textures"][c].GetString(), true))
		{
			std::cout << "\nDidn't load texture" << std::endl;
		}
	}
	// CubeMap Texture
	::g_pTextureManager->SetBasePath("assets/textures/cubemaps/");

	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("space",
		"SpaceBox_right1_posX.bmp", "SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp", "SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp", true, error_string))
		std::cout << "\nSpace skybox loaded" << std::endl;
	else
		std::cout << "\nskybox error: " << error_string << std::endl;
	//######################################### Loading Textures Here ################################################


	//##### GAME ### OBJECTS ### TO ### CREATED ### HERE ##################################################################
	size_t num_gameobjects = document["GameObjects"].Size();
	for (size_t c = 0; c < num_gameobjects; c++)
	{
		cGameObject* gameobject = new cGameObject();
		rapidjson::Value& jgameobj = document["GameObjects"][c];
		gameobject->physicsShapeType = (eShapeTypes)jgameobj["physicsshapetype"].GetInt();
		
		gameobject->meshName = jgameobj["meshname"].GetString();
		gameobject->friendlyName = jgameobj["friendlyname"].GetString();
		gameobject->m_position = glm::vec3(jgameobj["position"]["x"].GetFloat(),
			jgameobj["position"]["y"].GetFloat(),
			jgameobj["position"]["z"].GetFloat());
		gameobject->setOrientation(glm::vec3(
											jgameobj["rotation"]["x"].GetFloat(),
											jgameobj["rotation"]["y"].GetFloat(),
											jgameobj["rotation"]["z"].GetFloat()));
		gameobject->scale = jgameobj["scale"].GetFloat();
		gameobject->objectColourRGBA = glm::vec4(jgameobj["objectcolor"]["r"].GetFloat(),
												jgameobj["objectcolor"]["g"].GetFloat(),
												jgameobj["objectcolor"]["b"].GetFloat(),
												jgameobj["objectcolor"]["a"].GetFloat());
		gameobject->debugColour = glm::vec4(jgameobj["debugcolor"]["r"].GetFloat(),
											jgameobj["debugcolor"]["g"].GetFloat(),
											jgameobj["debugcolor"]["b"].GetFloat(),
											jgameobj["debugcolor"]["a"].GetFloat());
		for(int i=0;i<jgameobj["tex"].Size();i++)
		{
			gameobject->textures[i] = jgameobj["tex"][i].GetString();
			gameobject->textureRatio[i] = jgameobj["texratio"][i].GetFloat();
		}
		gameobject->objectType = (cGameObject::eObjectType)jgameobj["objectype"].GetInt();
		gameobject->GameObjectMesh = findMeshByName(vec_model_mesh, gameobject->meshName);
		gameobject->SPHERE_radius = jgameobj["sphereRadius"].GetFloat();
		gameobject->collision_radius = jgameobj["bulletCollisionRadius"].GetFloat();
		gameobject->isVisible = jgameobj["isVisible"].GetInt();
		gameobject->is_static = jgameobj["isStatic"].GetInt();
		g_vec_pGameObjects.push_back(gameobject);
	}
	//##### GAME ### OBJECTS ### TO ### CREATED ### HERE ##################################################################


	

	// Camera Created here
	::g_pFlyCamera = new cFlyCamera();
	::g_pFlyCamera->eye = glm::vec3(0.0f, 65.0, 340.0);
	::g_pFlyCamera->movementSpeed = 0.25f;
	::g_pFlyCamera->movementSpeed = 2.5f;
	// Camera Created here

	
	glEnable(GL_DEPTH);			// Write to the depth buffer
	glEnable(GL_DEPTH_TEST);	// Test with buffer when drawing

	
	//############################### Calculating DeltaTime #################################
	double last_time = glfwGetTime();   // Get the initial time
	//############################### Calculating DeltaTime #################################

	
	// Set up the FBO object
	p_fbo = new cFBO();
	// Usually we make this the size of the screen.
	std::string FBOError;
	if (p_fbo->init(1280, 720, FBOError))
	{
		std::cout << "Frame buffer is OK" << std::endl;
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

	//############################## Game Loop Starts Here ##################################################################
	while (!glfwWindowShouldClose(window))
	{
		//Draw everything to a Frame Bufer
		glBindFramebuffer(GL_FRAMEBUFFER, p_fbo->ID);

		p_fbo->clearBuffers(true, true);

		// Set the passNumber to 0
		GLint passNumber_UniLoc = glGetUniformLocation(shader_program_ID, "passNumber");
		glUniform1i(passNumber_UniLoc, 0);
		
		PhysicsInit();
		// Updating DeltaTime
			// Get the initial time
		double current_time = glfwGetTime();
		// Frame time... (how many seconds since last frame)
		double deltaTime = current_time - last_time;
		last_time = current_time;
		const double SOME_HUGE_TIME = 0.1;	// 100 ms;
		if (deltaTime > SOME_HUGE_TIME)
		{
			deltaTime = SOME_HUGE_TIME;
		}
		// Updating DeltaTime


		ProcessAsyncKeys(window);		// Listening to keyboard keys
		ProcessAsyncMouse(window);		// Listening to mouse

		glUseProgram(shader_program_ID);		// using current shader


		// Creating the GL_ViewPort   This is Pass 1
		float ratio;
		int width, height;
		glm::mat4 p, v;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
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
		glViewport(0, 0, width, height);

		
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear buffers


		////lights into shader
		p_light_stuff->loadLightIntoShader(shader_program_ID, vec_lightObjects.size());
		// camera into shader
		GLint eyeLocation_UL = glGetUniformLocation(shader_program_ID, "eyeLocation");
		glUniform4f(eyeLocation_UL,
			::g_pFlyCamera->getEye().x,
			::g_pFlyCamera->getEye().y,
			::g_pFlyCamera->getEye().z, 1.0f);
		//view and projection into shader
		GLint matView_UL = glGetUniformLocation(shader_program_ID, "matView");
		GLint matProj_UL = glGetUniformLocation(shader_program_ID, "matProj");
		glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
		glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

		// Window title Printing
		std::stringstream ssTitle;
		ssTitle
			<< g_pFlyCamera->eye.x << ", "
			<< g_pFlyCamera->eye.y << ", "
			<< g_pFlyCamera->eye.z
			<< "object postion: "
			<< g_vec_pGameObjects[4]->friendlyName<<" "
		<<"at vector player: "<<g_vec_pGameObjects[4]->m_at.x<<" "<< g_vec_pGameObjects[4]->m_at.y<<" "<< g_vec_pGameObjects[4]->m_at.z<<",ai: "
		<< g_vec_pGameObjects[5]->m_at.x<<" "<< g_vec_pGameObjects[5]->m_at.y<<" "<< g_vec_pGameObjects[5]->m_at.z;
		glfwSetWindowTitle(window, ssTitle.str().c_str());


		// GameObject Draw Call
		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		{
			cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];
			glm::mat4 matModel = glm::mat4(1.0f);	// Identity matrix
			
			if(pCurrentObject->m_physics_component)
			{
				pCurrentObject->m_physics_component->GetTransform(matModel);
			}
			else
			{

			}
			
			
			DrawObject(matModel, pCurrentObject,
				shader_program_ID, p_vao_manager);

		}//for (int index...


		// Maze Draw

		for(int a =0,draw1=0;a<maze_width-1;a++,draw1+=1)
			for(int b=0,draw2=0;b<maze_height-1;b++,draw2+=1)
			{
				
				if(p_maze_maker->maze[a][b][0] == true)
				{
					cGameObject* wall = findGameObjectByFriendlyName(g_vec_pGameObjects, "staticObject");
					glm::mat4 matModel = glm::mat4(1.0f);
					wall->m_position = glm::vec3(a+draw1, 50,b+draw2);
					DrawObject(matModel, wall,shader_program_ID, p_vao_manager);
				}
			}
		// Maze Draw
		
		//Physics implementation

		PhysicsUpdate(deltaTime);
		
		//Physics implementation



		//// This is Pass 2
		////The Whole scene is now drawn (to the FBO)
		//// 1. Disable the FBO
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//// 2. Clear the actual screen buffer
		//glViewport(0, 0, width, height);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//// 3. Use the FBO colour texture as the texture on the quad
		////glUniform1i(passNumber_UniLoc, 1);
		////Tie the texture to the texture unit
		//glActiveTexture(GL_TEXTURE22);				// Texture Unit 22
		//glBindTexture(GL_TEXTURE_2D, p_fbo->colourTexture_0_ID);	// Texture now assoc with texture unit 0
		////glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);
		//GLint textSamp00_UL = glGetUniformLocation(shader_program_ID, "secondPassColourTexture");
		//glUniform1i(textSamp00_UL, 22);	// Texture unit 22
		//// 4. Draw a single object ( a triangle or quad)
		//cGameObject* pQuadOrIsIt = findGameObjectByFriendlyName(g_vec_pGameObjects,"ground");
		//pQuadOrIsIt->isVisible = true;
		////pQuadOrIsIt->setOrientation(glm::vec3(-90, 0, 0));
		////pQuadOrIsIt->setRotationXYZ(glm::vec3(glm::radians(180.0f), 0, 0));
		//
		//*v = glm::lookAt(glm::vec3(0.0f, 100.0f, -300.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));
		//
		//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));*/
		//
		//// Set the actual screen size
		//GLint screenWidth_UnitLoc = glGetUniformLocation(shader_program_ID, "screenWidth");
		//GLint screenHeight_UnitLoc = glGetUniformLocation(shader_program_ID, "screenHeight");
		//
		//// Get the "screen" framebuffer size 
		//glfwGetFramebufferSize(window, &width, &height);
		//
		//glUniform1f(screenWidth_UnitLoc, width);
		//glUniform1f(screenHeight_UnitLoc, height);
		//
		//glm::mat4 matQuad = glm::mat4(1.0f);
		//DrawObject(matQuad, pQuadOrIsIt, shader_program_ID, p_vao_manager);

		// 2nd Pass  // Draw image on screen

		// 1. Set the framebuffer to the Actual screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Clear the screen (glClear())
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLint screenWidth_UnitLoc = glGetUniformLocation(shader_program_ID, "screenWidth");
		GLint screenHeight_UnitLoc = glGetUniformLocation(shader_program_ID, "screenHeight");
		glfwGetFramebufferSize(window, &width, &height);
		glUniform1f(screenWidth_UnitLoc, width);
		glUniform1f(screenHeight_UnitLoc, height);
		glViewport(0, 0, width, height);



		
		// 3. Set up the textures for the TV screen (From the FBO)
		glActiveTexture(GL_TEXTURE0 + 40);				// Texture Unit 40
		glBindTexture(GL_TEXTURE_2D, p_fbo->colourTexture_0_ID);	// Texture now asbsoc with texture unit 40      // Basically binding to
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);													// out vec4 pixelColor
		GLint color_pass_texture_UL = glGetUniformLocation(shader_program_ID, "secondPassColourTexture");             
		glUniform1i(color_pass_texture_UL, 40);	// Texture unit 40

		glActiveTexture(GL_TEXTURE0 + 41);				// Texture Unit 41
		glBindTexture(GL_TEXTURE_2D, p_fbo->normalTexture_ID);	// Texture now asbsoc with texture unit 41
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);
		GLint normal_pass_texture_UL = glGetUniformLocation(shader_program_ID, "secondPassNormalTexture");			// Basically binding to
		glUniform1i(normal_pass_texture_UL, 41);	// Texture unit 41												// out vec4 pixelNormal

		glActiveTexture(GL_TEXTURE0 + 42);				// Texture Unit 42
		glBindTexture(GL_TEXTURE_2D, p_fbo->depthTexture_ID);	// Texture now asbsoc with texture unit 42
		//glBindTexture(GL_TEXTURE_2D, pTheFBO->depthTexture_ID);
		GLint depth_pass_texture_UL = glGetUniformLocation(shader_program_ID, "secondPassDepthTexture");			
		glUniform1i(depth_pass_texture_UL, 42);	// Texture unit 42												
		
		
		// 4. Draw the TV and Screen
		glUniform1i(passNumber_UniLoc, 0);
		// GameObject Draw Call
		for (int index = 0; index != ::g_vec_pGameObjects.size(); index++)
		{
			cGameObject* pCurrentObject = ::g_vec_pGameObjects[index];
			glm::mat4 matModel = glm::mat4(1.0f);	// Identity matrix

			if (pCurrentObject->m_physics_component)
			{
				pCurrentObject->m_physics_component->GetTransform(matModel);
			}
			else
			{

			}

			if ((pCurrentObject->friendlyName) != "tvscreen1" && (pCurrentObject->friendlyName) != "tvscreen2")
			DrawObject(matModel, pCurrentObject,
				shader_program_ID, p_vao_manager);

		}//for (int index...
		//GLint passNumber_UniLoc = glGetUniformLocation(shader_program_ID, "passNumber");
		glUniform1i(passNumber_UniLoc, 2);
		cGameObject* p_TV_screen1 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen1");
		glm::mat4 mat4_TV_screen1 = glm::mat4(1.f);
		DrawObject(mat4_TV_screen1, p_TV_screen1, shader_program_ID, p_vao_manager);


		
		glUniform1i(passNumber_UniLoc, 4);
		cGameObject* p_TV_screen2 = findGameObjectByFriendlyName(g_vec_pGameObjects, "tvscreen2");
		glm::mat4 mat4_TV_screen2 = glm::mat4(1.f);
		DrawObject(mat4_TV_screen2, p_TV_screen2, shader_program_ID, p_vao_manager);

		// Place the camera somewhere stationary
		glUniform1i(passNumber_UniLoc, 0);


		glfwSwapBuffers(window);		// Buffer Swap
		glfwPollEvents();

	}
	//############################## Game Loop Ends Here ##################################################################




	// Destroy 
	glfwDestroyWindow(window);
	glfwTerminate();

	delete p_model_loader;
	delete p_shader_manager;
	delete p_vao_manager;
	delete g_pFlyCamera;
	delete g_pDebugRenderer;
	delete g_pTextureManager;
	delete p_maze_maker;
	/*delete pPhysics;*/
	PhysicsEnd();
	for (int i = 0; i < g_vec_pGameObjects.size(); i++)
		delete g_vec_pGameObjects[i];
	exit(EXIT_SUCCESS);

	return 0;
}




void DrawObject(glm::mat4 matModel,
	cGameObject* pCurrentObject,
	GLint shaderProgID,
	cVAOManager* pVAOManager)
{

	if (pCurrentObject->isVisible == false)
	{
		return;
	}


	// Turns on "alpha transparency"
	glEnable(GL_BLEND);

	// Reads what's on the buffer already, and 
	// blends it with the incoming colour based on the "alpha" 
	// value, which is the 4th colour output
	// RGB+A
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// This block of code, where I generate the world matrix from the 
	// position, scale, and rotation (orientation) of the object
	// has been placed into calculateWorldMatrix()


	// ************ 
	// Set the texture bindings and samplers

	// See if this is a skybox object? 
	GLint bIsSkyBox_UL = glGetUniformLocation(shaderProgID, "bIsSkyBox");
	if (pCurrentObject->friendlyName != "skybox")
	{
		// Is a regular 2D textured object
		SetUpTextureBindingsForObject(pCurrentObject, shaderProgID);
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
		glActiveTexture(GL_TEXTURE26);				// Texture Unit 26
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);	// Texture now assoc with texture unit 0

		// Tie the texture units to the samplers in the shader
		GLint skyBoxSampler_UL = glGetUniformLocation(shaderProgID, "skyBox");
		glUniform1i(skyBoxSampler_UL, 26);	// Texture unit 26
	}


	// ************

	glm::mat4 matWorldCurrentGO = calculateWorldMatrix(pCurrentObject, matModel);



	//uniform mat4 matModel;		// Model or World 
	//uniform mat4 matView; 		// View or camera
	//uniform mat4 matProj;
	GLint matModel_UL = glGetUniformLocation(shaderProgID, "matModel");

	glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, glm::value_ptr(matWorldCurrentGO));
	//glUniformMatrix4fv(matView_UL, 1, GL_FALSE, glm::value_ptr(v));
	//glUniformMatrix4fv(matProj_UL, 1, GL_FALSE, glm::value_ptr(p));

	// Calcualte the inverse transpose of the model matrix and pass that...
	// Stripping away scaling and translation, leaving only rotation
	// Because the normal is only a direction, really
	GLint matModelIT_UL = glGetUniformLocation(shaderProgID, "matModelInverseTranspose");
	glm::mat4 matModelInverseTranspose = glm::inverse(glm::transpose(matWorldCurrentGO));
	glUniformMatrix4fv(matModelIT_UL, 1, GL_FALSE, glm::value_ptr(matModelInverseTranspose));




	// Find the location of the uniform variable newColour
	GLint newColour_location = glGetUniformLocation(shaderProgID, "newColour");

	glUniform3f(newColour_location,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b);


	GLint diffuseColour_UL = glGetUniformLocation(shaderProgID, "diffuseColour");
	glUniform4f(diffuseColour_UL,
		pCurrentObject->objectColourRGBA.r,
		pCurrentObject->objectColourRGBA.g,
		pCurrentObject->objectColourRGBA.b,
		pCurrentObject->alphaTransparency);	// *********

	GLint specularColour_UL = glGetUniformLocation(shaderProgID, "specularColour");
	glUniform4f(specularColour_UL,
		1.0f,	// R
		1.0f,	// G
		1.0f,	// B
		1000.0f);	// Specular "power" (how shinny the object is)
					// 1.0 to really big (10000.0f)


//uniform vec4 debugColour;
//uniform bool bDoNotLight;
	GLint debugColour_UL = glGetUniformLocation(shaderProgID, "debugColour");
	GLint bDoNotLight_UL = glGetUniformLocation(shaderProgID, "bDoNotLight");

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
	//glPointSize(15.0f);

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


	//		glDrawArrays(GL_TRIANGLES, 0, 2844);
	//		glDrawArrays(GL_TRIANGLES, 0, numberOfVertsOnGPU);

	sModelDrawInfo drawInfo;
	//if (pTheVAOManager->FindDrawInfoByModelName("bunny", drawInfo))
	if (pVAOManager->FindDrawInfoByModelName(pCurrentObject->meshName, drawInfo))
	{
		glBindVertexArray(drawInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			drawInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}






	return;
} // DrawObject;

void SetUpTextureBindingsForObject(
	cGameObject* pCurrentObject,
	GLint shaderProgID)
{

	//// Tie the texture to the texture unit
	//GLuint texSamp0_UL = ::g_pTextureManager->getTextureIDFromName("Pizza.bmp");
	//glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
	//glBindTexture(GL_TEXTURE_2D, texSamp0_UL);	// Texture now assoc with texture unit 0

	// Tie the texture to the texture unit
	GLuint texSamp0_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->textures[0]);
	glActiveTexture(GL_TEXTURE0);				// Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, texSamp0_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp1_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->textures[1]);
	glActiveTexture(GL_TEXTURE1);				// Texture Unit 1
	glBindTexture(GL_TEXTURE_2D, texSamp1_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp2_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->textures[2]);
	glActiveTexture(GL_TEXTURE2);				// Texture Unit 2
	glBindTexture(GL_TEXTURE_2D, texSamp2_UL);	// Texture now assoc with texture unit 0

	GLuint texSamp3_UL = ::g_pTextureManager->getTextureIDFromName(pCurrentObject->textures[3]);
	glActiveTexture(GL_TEXTURE3);				// Texture Unit 3
	glBindTexture(GL_TEXTURE_2D, texSamp3_UL);	// Texture now assoc with texture unit 0

	// Tie the texture units to the samplers in the shader
	GLint textSamp00_UL = glGetUniformLocation(shaderProgID, "textSamp00");
	glUniform1i(textSamp00_UL, 0);	// Texture unit 0

	GLint textSamp01_UL = glGetUniformLocation(shaderProgID, "textSamp01");
	glUniform1i(textSamp01_UL, 1);	// Texture unit 1

	GLint textSamp02_UL = glGetUniformLocation(shaderProgID, "textSamp02");
	glUniform1i(textSamp02_UL, 2);	// Texture unit 2

	GLint textSamp03_UL = glGetUniformLocation(shaderProgID, "textSamp03");
	glUniform1i(textSamp03_UL, 3);	// Texture unit 3


	GLint tex0_ratio_UL = glGetUniformLocation(shaderProgID, "tex_0_3_ratio");
	glUniform4f(tex0_ratio_UL,
		pCurrentObject->textureRatio[0],		// 1.0
		pCurrentObject->textureRatio[1],
		pCurrentObject->textureRatio[2],
		pCurrentObject->textureRatio[3]);

	//{
	//	//textureWhatTheWhat
	//	GLuint texSampWHAT_ID = ::g_pTextureManager->getTextureIDFromName("WhatTheWhat.bmp");
	//	glActiveTexture(GL_TEXTURE13);				// Texture Unit 13
	//	glBindTexture(GL_TEXTURE_2D, texSampWHAT_ID);	// Texture now assoc with texture unit 0

	//	GLint textureWhatTheWhat_UL = glGetUniformLocation(shaderProgID, "textureWhatTheWhat");
	//	glUniform1i(textureWhatTheWhat_UL, 13);	// Texture unit 13
	//}



	return;
}

glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject, glm::mat4 matWorld)
{

	//glm::mat4 matWorld = glm::mat4(1.0f);


	//// ******* TRANSLATION TRANSFORM *********
	//glm::mat4 matTrans
	//	= glm::translate(glm::mat4(1.0f),
	//		glm::vec3(pCurrentObject->m_physics_component->getPosition().x,
	//			pCurrentObject->m_physics_component->getPosition().y,
	//			pCurrentObject->m_physics_component->getPosition().z));
	//matWorld = matWorld * matTrans;

	//matWorld =  pCurrentObject->DoRender();

	if(pCurrentObject->is_static == 1)
	{	
		glm::mat4 matTrans = glm::translate(glm::mat4(1.f), pCurrentObject->m_position);
		matWorld = matWorld * matTrans;
	}
	

	glm::mat4 matRotation = glm::mat4(pCurrentObject->getQOrientation());
	matWorld = matWorld * matRotation;
	//}
	// ******* ROTATION TRANSFORM *********

	// ******* SCALE TRANSFORM *********
	glm::mat4 scale = glm::scale(glm::mat4(1.0f),
		glm::vec3(pCurrentObject->scale,
			pCurrentObject->scale,
			pCurrentObject->scale));
	matWorld = matWorld * scale;
	// ******* SCALE TRANSFORM *********

// ******* TRANSLATION TRANSFORM *********

	//// ******* ROTATION TRANSFORM *********
	//if ( pCurrentObject->friendlyName != "StarDestroyer" )
	//{ 
	//	//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
	//	glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
	//									pCurrentObject->rotationXYZ.z,					// Angle 
	//									glm::vec3(0.0f, 0.0f, 1.0f));
	//	matWorld = matWorld * rotateZ;
//
	//	glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
	//									pCurrentObject->rotationXYZ.y,	//(float)glfwGetTime(),					// Angle 
	//									glm::vec3(0.0f, 1.0f, 0.0f));
	//	matWorld = matWorld * rotateY;
//
	//	glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
	//									pCurrentObject->rotationXYZ.x,	// (float)glfwGetTime(),					// Angle 
	//									glm::vec3(1.0f, 0.0f, 0.0f));
	//	matWorld = matWorld * rotateX;
	//}
	//else
	//{
	//	// HACK: Adjust the rotation of the ship.
	//	glm::quat qAdjust = glm::quat( glm::vec3(0.0f, glm::radians(0.1f), 0.0f )); 
	//	pCurrentObject->qRotation = pCurrentObject->qRotation * qAdjust;
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
