#include "shapes.h"

float triangleVertices[] = {
				//Position													//Color									//UV
				-0.5, -0.5, 0,									1, 1, 1,								0, 0,
				0.5, -0.5, 0,										1, 1, 1,								1, 0,
				0, 0.5, 0,													1, 1, 1,								1, 0
};
unsigned int triangleIndices[] = {
	0, 1, 2
};
size_t triangleVerticeSize = sizeof(triangleVertices);
size_t triangleIndiceSize = sizeof(triangleIndices);


float squareVertices[] = {
				//Position														//Color									//UV
					-1, 1, 0,														1, 1, 1,								0, 1,
					1, 1, 0,															1, 1, 1,								1, 1,
					1, -1, 0,														1, 1, 1,								1, 0,
					-1, -1, 0,													1, 1, 1,								0, 0
};
unsigned int squareIndices[] = {
	0, 2, 1,
	0, 3, 2
};
size_t squareVerticeSize = sizeof(squareVertices);
size_t squareIndiceSize = sizeof(squareIndices);


float cubeVertices[] = {
				//Position														//Color									//UV
				-0.5f, -0.5f, -0.5f,				1, 1, 1,								0, 0,
				-0.5f, 0.5f, -0.5f,					1, 1, 1,								0, 0,
					0.5f, -0.5f, -0.5f,				1, 1, 1,								0, 0,
				0.5f, 0.5f, -0.5f,						1, 1, 1,								0, 0,

				-0.5f, -0.5f, 0.5f,					1, 1, 1,								0, 0,
				-0.5f, 0.5f, 0.5f,						1, 1, 1,								0, 0,
				0.5f, -0.5f, 0.5f,						1, 1, 1,								0, 0,
				0.5f, 0.5f, 0.5f,							1, 1, 1,								0, 0
};

unsigned int cubeIndices[] = {
				0, 1, 2,
				2, 1, 3,

				0, 4, 2,
				4, 2, 6,

				1, 7, 2,
				2, 7, 6,

				0, 5, 1,
				0, 4, 5,

				1, 5, 3,
				3, 5, 7,

				4, 6, 5,
				6, 5, 7
};
size_t cubeVerticeSize = sizeof(cubeVertices);
size_t cubeIndiceSize = sizeof(cubeIndices);