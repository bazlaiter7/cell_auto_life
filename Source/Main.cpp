#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <iostream>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "UI/UI.h"
#include "Window/Window.h"
#include "GameBoard.h"
#include "Controll/Controll.h"
#include "Data/Data.h"
#include "string"
#include <audiere/audiere.h>


// application classes declaration
Window app_window;
Data app_data;
UI app_ui;

audiere::AudioDevicePtr device = audiere::OpenDevice();





std::string console = "test\nthis is console data!\n";
std::string* console_ptr = &console;


int population = 10;
int buff_population = 0;
std::string population_result_string = "population: 0";
int* population_ptr = &population;


//Global variables
GameBoard board(_SCREEN_WIDTH_MAX, _SCREEN_HEIGHT_MAX, _CELL_SIZE);

//If the left or right click has been pressed
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (app_data.glfw_main_app_focus) {
		
		//If the left click has been pressed
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
				app_data.leftClickDown = true;
			else if (action == GLFW_RELEASE)
				app_data.leftClickDown = false;
		}

		//If the right click has been pressed
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS)
				app_data.rightClickDown = true;
			else if (action == GLFW_RELEASE)
				app_data.rightClickDown = false;
		}
		ImGui::SetCurrentContext(app_data.main_context);
		ImGui_ImplGlfw_MouseButtonCallback(window, NULL, NULL, NULL);
	}
	else {
		ImGui::SetCurrentContext(app_data.main_context);
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	}
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//'R' key resets the board
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		board.ResetBoard(console_ptr, population_ptr);

	//'Space' key pauses/unpauses the board
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		app_data.paused = !app_data.paused;

	//'+' key increases the board speed
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
	{
		app_data.frameSpeed /= 2;
		if(app_data.frameSpeed < _INITIAL_FRAMESPEED / 16)
			app_data.frameSpeed = _INITIAL_FRAMESPEED / 16;
	}

	//'-' key increases the board speed
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
	{
		app_data.frameSpeed *= 2;
		if (app_data.frameSpeed > _INITIAL_FRAMESPEED * 4)
			app_data.frameSpeed = _INITIAL_FRAMESPEED * 4;
	}

	//'esc' or 'q' key closes the program
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) 
	{
		app_data.hasQuit = true;
		glfwTerminate();
	}

	//'1' to '5' load a premade sequence
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		board.LoadPreconfiguration(1, console_ptr, population_ptr, device);
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		board.LoadPreconfiguration(2, console_ptr, population_ptr, device);
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		board.LoadPreconfiguration(3, console_ptr, population_ptr, device);
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		board.LoadPreconfiguration(4, console_ptr, population_ptr, device);
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		board.LoadPreconfiguration(5, console_ptr, population_ptr, device);
}

int main(void)
{
	//-----------------Initialize openGL window------------------------------//
	if (!device) {
		std::cerr << "Ошибка при открытии аудиоустройстваn";
		
	}

	audiere::OutputStreamPtr sound(audiere::OpenSound(device, "./birth_sound.wav", true));
	if (!sound) {
		std::cerr << "Ошибка при открытии звукового файлаn";
		return 1;
	}

	if (sound) {
		sound->play();
	}
	else {
		std::cerr << "Ошибка: указатель sound равен nullptrn";
	}
	
	app_window.init_window(_SCREEN_WIDTH_MAX, _SCREEN_HEIGHT_MAX, "Programm IVT", NULL, NULL);
	app_ui.init_ImGUI(app_window.get_window(), app_data.main_context);


	glfwSetKeyCallback(app_window.get_window(), key_callback); //Keyboard event

	//-----------------Initialize Game of Life varianbles--------------------//
	double xpos, ypos; //The x and y of the mouse
	int width = _SCREEN_WIDTH_MAX, height = _SCREEN_HEIGHT_MAX; //The current width and height of the window

	//Give the board an initial pattern
	board.LoadPreconfiguration(0, console_ptr, population_ptr, device);
	
	//Loop until the user closes the window
	while (!glfwWindowShouldClose(app_window.get_window()))
	{
		//Get the position of the mouse
		glfwGetCursorPos(app_window.get_window(), &xpos, &ypos);

		//Enable/disable the cells that the player left/right clicks on
		if (app_data.leftClickDown) 
		{
			board.MouseClick(xpos, ypos, true, population_ptr, console_ptr, device);
		}
		else if (app_data.rightClickDown)
		{
			board.MouseClick(xpos, ypos, false, population_ptr, console_ptr, device);
		}

		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glClearColor(1, 1, 1, 1); //Set the background color to all white
		
		//Drawing
		board.DrawBoard(console_ptr, population_ptr); //Draw the board
		board.DrawGrid(); //Draw the gridlines

		//Draw the cell outline we're hovering over
		board.DrawMouseHover(xpos, ypos);

		//Calculate the next frame if a certain amount of time has elapsed
		if (glfwGetTime() > app_data.frameSpeed && !app_data.paused)
		{
			//Check if the window has been resized
			int newWidth, newHeight;
			glfwGetFramebufferSize(app_window.get_window(), &newWidth, &newHeight);
			
			if (newWidth != width || newHeight != height) 
			{
				board.ChangedWindowSize(newWidth, newHeight);

				width = newWidth;
				height = newHeight;
			}

			board.CalculateNextFrame(console_ptr, population_ptr);
			glfwSetTime(0);
		}

		ImGui::Begin("Console", NULL);
		{
			if (ImGui::IsWindowHovered()) {
				app_data.glfw_main_app_focus = false;
				
			}
			else {
				app_data.glfw_main_app_focus = true;
				//Event handlers
				glfwSetMouseButtonCallback(app_window.get_window(), mouse_button_callback); //Mouseclick event
			}
		}
		population_result_string = "population: " + population;
		ImGui::Text("population: ");
		ImGui::Text(std::to_string(population).c_str());
		ImGui::Text(console.c_str());
	
		ImGui::End();
		ImGui::Render();

		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


		//Double buffer
		glfwSwapBuffers(app_window.get_window());

		//Poll for events
		glfwPollEvents();

		if (app_data.hasQuit)
			return 0;
	}

	app_ui.shutdown_ImGUI();
	app_window.terminate_work();
	return 0;
}