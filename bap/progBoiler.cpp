#include "progBoiler.h"
#include <iostream>

extern Camera _mainCamera;

namespace Movement {
				void camera(float speed) {
								_mainCamera.setRotation(-_mousePosY * speed, (_mousePosX) * speed, 0);
				}

				void freeMove(float speed) {
								if (keyAction::keyHeld(GLFW_KEY_SPACE))
												_mainCamera.position.y += speed * _deltaTime;
								if (keyAction::keyHeld(GLFW_KEY_LEFT_CONTROL))
												_mainCamera.position.y += -speed * _deltaTime;


								if (keyAction::keyHeld(GLFW_KEY_W))
												_mainCamera.position += _mainCamera.forwardDirection() * speed * _deltaTime;
								if (keyAction::keyHeld(GLFW_KEY_S))
												_mainCamera.position += _mainCamera.forwardDirection() * -speed * _deltaTime;

								if (keyAction::keyHeld(GLFW_KEY_A))
												_mainCamera.position += _mainCamera.rightDirection() * -speed * _deltaTime;
								if (keyAction::keyHeld(GLFW_KEY_D))
												_mainCamera.position += _mainCamera.rightDirection() * speed * _deltaTime;
				}
}