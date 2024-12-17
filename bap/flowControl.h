#pragma once
#include "vector.h"

void setBgColor(Vec4 color);
void setBgColor(float r, float g, float b, float a);

#if defined(_FLOW_CONTROL) || defined(_PROGRAM_CONTROL)
#include "openGL.h"
void start();
void update();
void end();

void windowMoveCallback(GLFWwindow* window, int xpos, int ypos);
void windowScaleCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff);
#endif


#if defined(_FLOW_CONTROL)
#include "typeDefs.h"
#include "objects.h"
#include "openGL.h"
#include "shapes.h"
#include "GlobalVars.h"
#include <iostream>
#include "Input.h"
#include "constants.h"
#include "mathConstants.h"
#include "Text.h"
#include <windows.h>
#include "progUI.h"
#include "stb_image.h"
#include "soundLoading.h"
#include "modelLoading.h"
#endif