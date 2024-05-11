#pragma once
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "string"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"


class UI {
private:
	std::string console = "";
public:
	void add_console_date(std::string data);
	std::string get_console();
	void init_ImGUI(GLFWwindow* window, ImGuiContext* context);
	void shutdown_ImGUI();
};