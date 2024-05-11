#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <GLFW/glfw3.h>
#define _SCREEN_WIDTH_MAX 1600
#define _SCREEN_HEIGHT_MAX 900
#define _SCREEN_WIDTH_MIN 1050
#define _SCREEN_HEIGHT_MIN 500
#define _CELL_SIZE 20
#define _INITIAL_FRAMESPEED 0.1


class Data {
public:
	bool leftClickDown = false,
		rightClickDown = false,
		glfw_main_app_focus = true,
		paused = false,
		hasQuit = false,
		frameSpeed = _INITIAL_FRAMESPEED;
	ImGuiContext* main_context = ImGui::CreateContext();
	ImGuiContext* secondary_context = ImGui::CreateContext();
	char geg = 'd';
	char* buf = &geg;
};