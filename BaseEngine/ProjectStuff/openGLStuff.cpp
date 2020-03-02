#include"openGLStuff.h"
#include"../FlyCamera/cFlyCamera.h"
#include "../GameObject/cGameObject.h"
#include "../global.h"
#include "../Animations/cAnimationState.h"

bool g_MouseIsInsideWindow = false;
bool g_MouseLeftButtonIsDown = false;
bool on_path = false;
bool revesre = false;
bool is_flock = false;
int currentSphere = 6;
int changePlayer = 0;
int punchcounter = 0;
extern cFlyCamera* g_pFlyCamera;
extern nPhysics::iPhysicsWorld* physics_world;
extern bool changePhys;
extern int dataLoaded;
extern std::string g_HACK_currentAnimationName;
extern float HACK_FrameTime;

bool isOnlyShiftKeyDown(int mods);
bool isOnlyCtrlKeyDown(int mods);
bool isOnlyAltKeyDown(int mods);
bool isShiftDown(GLFWwindow* window);
bool isCtrlDown(GLFWwindow* window);
bool isAltDown(GLFWwindow* window);
bool areAllModifiersUp(GLFWwindow* window);

extern std::vector<cGameObject*> g_vec_pGameObjects;

extern cGameObject* findGameObjectByFriendlyName(std::vector<cGameObject*> vGameObjects, std::string friendlyname);

const float MOVESPEED = 150.0f;


// openGLfunc
GLFWwindow* creatOpenGL(GLFWwindow* win)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	win = glfwCreateWindow(1280, 720, "patterns and framework", NULL, NULL);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(win, key_callback);
	glfwMakeContextCurrent(win);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	glfwSetCursorPosCallback(win, cursor_position_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetCursorEnterCallback(win, cursor_enter_callback);
	glfwSetScrollCallback(win, scroll_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	return win;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cGameObject* player;
	if(changePlayer == 0)
	{
		player = findGameObjectByFriendlyName(g_vec_pGameObjects, "player");
	}
	else
	{
		player = findGameObjectByFriendlyName(g_vec_pGameObjects, "rpgchar2"); 
	}
	cGameObject* current_sphere_in_control = g_vec_pGameObjects[currentSphere];
	//if(isOnlyCtrlKeyDown(mods))
	//if (areAllModifiersUp(window))
	if(!isAltDown(window) && !isCtrlDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_0))		// Turn on/off path following
		{
			on_path = !on_path;
			physics_world->GetOnPath(on_path);
			
		}
		if (glfwGetKey(window, GLFW_KEY_6))		// Turn on Flock
		{
			is_flock = true;
			physics_world->GetIsFlock(is_flock);

		}
		if (glfwGetKey(window, GLFW_KEY_7))		// Turn off Flock
		{
			is_flock = false;
			physics_world->GetIsFlock(is_flock);

		}
		if (glfwGetKey(window, GLFW_KEY_9))		// Reverse the path
		{
			revesre = !revesre;
			physics_world->GetIsReverse(revesre);

		}
		if (glfwGetKey(window, GLFW_KEY_1))		// Formation 1 Circle
		{
			physics_world->GetFormationType(0);
		}
		if (glfwGetKey(window, GLFW_KEY_2))		// Formation 2 V
		{
			physics_world->GetFormationType(1);
			
		}
		if (glfwGetKey(window, GLFW_KEY_3))		// Formation 1 Square
		{
			physics_world->GetFormationType(2);
			
		}
		if (glfwGetKey(window, GLFW_KEY_4))		// Formation 1 Line
		{
			
			physics_world->GetFormationType(3);
		}
		if (glfwGetKey(window, GLFW_KEY_5))		// Formation 1 Rows
		{
			physics_world->GetFormationType(4);
			
		}
		//if(glfwGetKey(window, GLFW_KEY_W))		// walk forward
		//{
		//	//player->updateAtFromOrientation();
		//	//player->velocity.z += MOVESPEED;
		//	player->m_physics_component->ApplyForce(glm::vec3(0,0,MOVESPEED));
		//	
		//}
		//if(glfwGetKey(window, GLFW_KEY_S))		// walk backward
		//{
		//	//player->updateAtFromOrientation();
		//	player->m_physics_component->ApplyForce(glm::vec3(0,0,-MOVESPEED));
		//	//player->MoveForward_Z(-MOVESPEED);
		//}
		//if (glfwGetKey(window, GLFW_KEY_D))
		//{
		//	//player->updateOrientation(glm::vec3(0, -1, 0));
		//	player->m_physics_component->ApplyRotation(glm::vec3(0, -1, 0));
		//	//player->updateAtFromOrientation();
		//}
		//	
		//if (glfwGetKey(window, GLFW_KEY_A))
		//{
		//	player->m_physics_component->ApplyRotation(glm::vec3(0, 1, 0));
		//	//player->updateOrientation(glm::vec3(0, 1, 0));
		////	player->updateAtFromOrientation();
		//}
		/*if (glfwGetKey(window, GLFW_KEY_Q))
		{
			
		}
		if (glfwGetKey(window, GLFW_KEY_E))
		{
			
		}
		

		if(glfwGetKey(window,GLFW_KEY_SPACE) && action == GLFW_PRESS)
		{
		
		}*/
		/*if(glfwGetKey(window,GLFW_KEY_N) && action == GLFW_PRESS)
		{

			changePlayer = changePlayer==1?0:1;
			
			
		}*/
		/*if (glfwGetKey(window, GLFW_KEY_1) && action == GLFW_PRESS)
		{
			PhysicsEnd();
			changePhys = !changePhys;
			dataLoaded = 0;
		}*/
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	return;
}

void ProcessAsyncMouse(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	::g_pFlyCamera->setMouseXY(x, y);
	//::g_pCameraManager->getCurrentCamera()->Update("setMouseXY", glm::vec2(x, y));

	const float MOUSE_SENSITIVITY = 0.1f;

	//	std::cout << ::g_pFlyCamera->getMouseX() << ", " << ::g_pFlyCamera->getMouseY() << std::endl;

		// Mouse left (primary?) button pressed? 
		// AND the mouse is inside the window...
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		&& ::g_MouseIsInsideWindow)
	{
		// Mouse button is down so turn the camera
		::g_pFlyCamera->Yaw_LeftRight(-::g_pFlyCamera->getDeltaMouseX() * MOUSE_SENSITIVITY);
		//glm::vec3 deltaMouseX = ::g_pCameraManager->getCurrentCamera()->getData("getDeltaMouseX");
		//::g_pCameraManager->getCurrentCamera()->Update("Yaw_LeftRight", -deltaMouseX.x * MOUSE_SENSITIVITY);


		::g_pFlyCamera->Pitch_UpDown(::g_pFlyCamera->getDeltaMouseY() * MOUSE_SENSITIVITY);
		//glm::vec3 deltaMouseY = ::g_pCameraManager->getCurrentCamera()->getData("getDeltaMouseY");
		//::g_pCameraManager->getCurrentCamera()->Update("Pitch_UpDown", deltaMouseY.x * MOUSE_SENSITIVITY);

	}

	// Adjust the mouse speed
	if (::g_MouseIsInsideWindow)
	{
		const float MOUSE_WHEEL_SENSITIVITY = 0.1f;

		// Adjust the movement speed based on the wheel position
		::g_pFlyCamera->movementSpeed += (::g_pFlyCamera->getMouseWheel() * MOUSE_WHEEL_SENSITIVITY);

		//glm::vec3 movementSpeed = ::g_pCameraManager->getCurrentCamera()->getData("getMovementSpeed");

		//glm::vec3 mouseWheel = ::g_pCameraManager->getCurrentCamera()->getData("getMouseWheel");
		//movementSpeed.x += mouseWheel.x * MOUSE_WHEEL_SENSITIVITY;
		//::g_pCameraManager->getCurrentCamera()->Update("setMovementSpeed", mouseWheel.x * MOUSE_WHEEL_SENSITIVITY );



		if (::g_pFlyCamera->movementSpeed <= 0.0f)
		{
			::g_pFlyCamera->movementSpeed = 0.0f;
		}
		//if (movementSpeed.x <= 0.0f)
		//{
		//	movementSpeed.x = 0.0f;
		//}
		//
	}


	// HACK 
	//::g_pFlyCamera->movementSpeed = 2.0f;
	//::g_pCameraManager->getCurrentCamera()->Update("setMovementSpeed", 2.0f);

	return;
}//ProcessAsyncMouse(...

void ProcessAsyncKeys(GLFWwindow* window)
{
	const float CAMERA_MOVE_SPEED_SLOW = 0.1f;
	const float CAMERA_MOVE_SPEED_FAST = 1.0f;

	const float CAMERA_TURN_SPEED = 0.1f;

	// WASD + q = "up", e = down		y axis = up and down
	//									x axis = left and right
	//									z axis = forward and backward
	// 

	//float cameraSpeed = CAMERA_MOVE_SPEED_SLOW;
	//if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS  )
	//{
	//	cameraSpeed = CAMERA_MOVE_SPEED_FAST;
	//}

	float cameraMoveSpeed = ::g_pFlyCamera->movementSpeed;
	//float cameraMoveSpeed = ::g_pCameraManager->getCurrentCamera()->getData("getMovementSpeed").x;



	// If no keys are down, move the camera
	//if (areAllModifiersUp(window))
	if(isCtrlDown(window))
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			//			g_CameraEye.z += cameraSpeed;
			::g_pFlyCamera->MoveForward_Z(+cameraMoveSpeed);
			//::g_pCameraManager->getCurrentCamera()->Update("MoveForward_Z", +cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			//			g_CameraEye.z -= cameraSpeed;
			::g_pFlyCamera->MoveForward_Z(-cameraMoveSpeed);
			//::g_pCameraManager->getCurrentCamera()->Update("MoveForward_Z", -cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			//			g_CameraEye.x -= cameraSpeed;
			::g_pFlyCamera->MoveLeftRight_X(-cameraMoveSpeed);
			//::g_pCameraManager->getCurrentCamera()->Update("MoveLeftRight_X", -cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			//			g_CameraEye.x += cameraSpeed;
			::g_pFlyCamera->MoveLeftRight_X(+cameraMoveSpeed);
			//::g_pCameraManager->getCurrentCamera()->Update("MoveLeftRight_X", +cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			//			::g_pFlyCamera->Roll_CW_CCW( +cameraSpeed );
			::g_pFlyCamera->MoveUpDown_Y(-cameraMoveSpeed);
			//::g_pCameraManager->getCurrentCamera()->Update("MoveUpDown_Y", -cameraMoveSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			//			g_CameraEye.y -= cameraSpeed;
			//			::g_pFlyCamera->Roll_CW_CCW( -cameraSpeed );
			::g_pFlyCamera->MoveUpDown_Y(+cameraMoveSpeed);
			//::g_pCameraManager->getCurrentCamera()->Update("MoveUpDown_Y", +cameraMoveSpeed);
		}
	}//if(AreAllModifiersUp(window)


	// Note: if tge fly camera has bKeepCameraFacingUp = true, then this is ignored
	if (isAltDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			//			::g_pFlyCamera->MoveUpDown_Y( +cameraSpeed );
			::g_pFlyCamera->Roll_CW_CCW(-CAMERA_TURN_SPEED);
			//::g_pCameraManager->getCurrentCamera()->Update("Roll_CW_CCW", -CAMERA_TURN_SPEED);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			//			::g_pFlyCamera->MoveUpDown_Y( -cameraSpeed );
			::g_pFlyCamera->Roll_CW_CCW(+CAMERA_TURN_SPEED);
			//::g_pCameraManager->getCurrentCamera()->Update("Roll_CW_CCW", +CAMERA_TURN_SPEED);
		}
	}//IsShiftDown(window)


	return;
}// ProcessAsyncKeys(..



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// A regular mouse wheel returns the y value
	//::g_pFlyCamera->setMouseWheelDelta(yoffset * 0.1f);
	//g_pCameraManager->getCurrentCamera()->Update("setMouseWheelDelta", (float)yoffset);

	//	std::cout << "Mouse wheel: " << yoffset << std::endl;

	return;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		::g_MouseLeftButtonIsDown = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		::g_MouseLeftButtonIsDown = false;
	}

	return;
}

void cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered)
	{
		::g_MouseIsInsideWindow = true;
		std::cout << "Mouse moved inside window" << std::endl;
	}
	else
	{
		::g_MouseIsInsideWindow = false;
		std::cout << "Mouse moved outside window" << std::endl;
	}
	return;
}//cursor_enter_callback(...

bool isOnlyShiftKeyDown(int mods)
{
	if (mods == GLFW_MOD_SHIFT)
	{
		// Shift key is down all by itself
		return true;
	}
	//// Ignore other keys and see if the shift key is down
	//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
	//{

	//}
	return false;
}

bool isOnlyCtrlKeyDown(int mods)
{
	if (mods == GLFW_MOD_CONTROL)
	{
		return true;
	}
	return false;
}

bool isOnlyAltKeyDown(int mods)
{
	if (mods == GLFW_MOD_ALT)
	{
		return true;
	}
	return false;
}

bool isShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

// openGLfunc