#pragma once
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow* window;
	int initial_width, initial_height;
public:
	// setter
	void set_initial_width(int initial_width);
	void set_initial_height(int initial_height);
	void set_window(GLFWwindow* window);
	// getter
	int get_initial_width();
	int get_initial_height();
	GLFWwindow* get_window();
	
	// main methods
	void init_window(int window_width, 
		int window_height, 
		const char* window_title, 
		GLFWmonitor* monitor, 
		GLFWwindow* share
	);
	void terminate_work();
	void draw();
};