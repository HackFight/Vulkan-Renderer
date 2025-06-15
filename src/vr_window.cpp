#include "vr_window.hpp"

#include <stdexcept>

namespace vr
{
	VrWindow::VrWindow(int w, int h, std::string n) : width{ w }, height{ h }, name{ n }
	{
		InitWindow();
	}

	VrWindow::~VrWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void VrWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

	void VrWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	}
}