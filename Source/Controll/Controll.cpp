#include "Controll.h"





//void Controll::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//	//'R' key resets the board
//	if (key == GLFW_KEY_R && action == GLFW_PRESS)
//		board.ResetBoard();
//
//	//'Space' key pauses/unpauses the board
//	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
//		paused = !paused;
//
//	//'+' key increases the board speed
//	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
//	{
//		frameSpeed /= 2;
//		if (frameSpeed < _INITIAL_FRAMESPEED / 16)
//			frameSpeed = _INITIAL_FRAMESPEED / 16;
//	}
//
//	//'-' key increases the board speed
//	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
//	{
//		frameSpeed *= 2;
//		if (frameSpeed > _INITIAL_FRAMESPEED * 4)
//			frameSpeed = _INITIAL_FRAMESPEED * 4;
//	}
//
//	//'esc' or 'q' key closes the program
//	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
//	{
//		hasQuit = true;
//		glfwTerminate();
//	}
//
//	//'1' to '5' load a premade sequence
//	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
//		board.LoadPreconfiguration(1);
//	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
//		board.LoadPreconfiguration(2);
//	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
//		board.LoadPreconfiguration(3);
//	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
//		board.LoadPreconfiguration(4);
//	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
//		board.LoadPreconfiguration(5);
//}


//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (app_data.glfw_main_app_focus) {
//
//		//If the left click has been pressed
//		if (button == GLFW_MOUSE_BUTTON_LEFT)
//		{
//			if (action == GLFW_PRESS)
//				app_data.leftClickDown = true;
//			else if (action == GLFW_RELEASE)
//				app_data.leftClickDown = false;
//		}
//
//		//If the right click has been pressed
//		if (button == GLFW_MOUSE_BUTTON_RIGHT)
//		{
//			if (action == GLFW_PRESS)
//				app_data.rightClickDown = true;
//			else if (action == GLFW_RELEASE)
//				app_data.rightClickDown = false;
//		}
//		ImGui::SetCurrentContext(app_data.main_context);
//		ImGui_ImplGlfw_MouseButtonCallback(window, NULL, NULL, NULL);
//	}
//	else {
//		ImGui::SetCurrentContext(app_data.main_context);
//		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
//	}
//}