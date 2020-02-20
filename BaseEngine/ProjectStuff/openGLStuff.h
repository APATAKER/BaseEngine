#ifndef _openGLStuff_HG_
#define _openGLStuff_HG_
#include"../commonHeaders.h"



GLFWwindow* creatOpenGL(GLFWwindow* win);


void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);

void ProcessAsyncMouse(GLFWwindow* window);
void ProcessAsyncKeys(GLFWwindow* window);
#endif // !_openGLStuff_HG_

