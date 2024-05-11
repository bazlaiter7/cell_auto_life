#include "Window.h"
#include "../UI/UI.h"
#include <GLFW/glfw3.h>


GLFWwindow* Window::get_window() {
	return this->window;
}

void Window::set_window(GLFWwindow* window) {
	this->window = window;
}

void Window::init_window(int window_width,
	int window_height,
	const char* window_title,
	GLFWmonitor* monitor,
	GLFWwindow* share)
{	
	GLFWwindow* window;
	if (!glfwInit()) {
		return;
	}
	window = glfwCreateWindow(window_width, window_height, window_title, monitor, share);
	this->window = window;
	this->initial_height = window_height;
	this->initial_width = window_width;
	if (!window){
		glfwTerminate();
		return;
	}
	glfwSetWindowSizeLimits(window, window_width, window_height, window_width, window_height);
	glfwMakeContextCurrent(window);
}

void Window::terminate_work() {
	glfwTerminate();
}

void Window::draw() {
	//while (!glfwWindowShouldClose(this->window))
	//{
	//	//Get the position of the mouse
	//	glfwGetCursorPos(this->window, &xpos, &ypos);

	//	//Enable/disable the cells that the player left/right clicks on
	//	if (leftClickDown)
	//	{
	//		board.MouseClick(xpos, ypos, true);
	//	}
	//	else if (rightClickDown)
	//	{
	//		board.MouseClick(xpos, ypos, false);
	//	}

	//	//Clear the screen
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	ImGui_ImplOpenGL2_NewFrame();
	//	ImGui_ImplGlfw_NewFrame();
	//	ImGui::NewFrame();

	//	glClearColor(1, 1, 1, 1); //Set the background color to all white

	//	//Drawing
	//	board.DrawBoard(); //Draw the board
	//	board.DrawGrid(); //Draw the gridlines

	//	//Draw the cell outline we're hovering over
	//	board.DrawMouseHover(xpos, ypos);

	//	//Calculate the next frame if a certain amount of time has elapsed
	//	if (glfwGetTime() > frameSpeed && !paused)
	//	{
	//		//Check if the window has been resized
	//		int newWidth, newHeight;
	//		glfwGetFramebufferSize(this->window, &newWidth, &newHeight);

	//		if (newWidth != this->initial_width || newHeight != this->initial_height)
	//		{
	//			board.ChangedWindowSize(newWidth, newHeight);

	//			this->initial_width = newWidth;
	//			this->initial_height = newHeight;
	//		}

	//		board.CalculateNextFrame();
	//		glfwSetTime(0);
	//	}

	//	ImGui::Begin("scene");
	//	{
	//		if (ImGui::IsWindowHovered()) {
	//			glfw_main_app_focus = false;

	//		}
	//		else {
	//			glfw_main_app_focus = true;
	//			//Event handlers
	//			glfwSetMouseButtonCallback(app_window.get_window(), mouse_button_callback); //Mouseclick event
	//		}
	//	}
	//	ImGui::Text("Hello, world %d", 123);

	//	float f = 1.0f;

	//	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

	//	ImGui::End();
	//	ImGui::Render();

	//	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


	//	//Double buffer
	//	glfwSwapBuffers(app_window.get_window());

	//	//Poll for events
	//	glfwPollEvents();

	//	if (hasQuit)
	//		return 0;
	//}
}