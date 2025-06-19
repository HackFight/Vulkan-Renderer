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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FrameBufferResizedCallback);
	}

	void VrWindow::FrameBufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto vrWindow = reinterpret_cast<VrWindow*>(glfwGetWindowUserPointer(window));
		vrWindow->frameBufferResized = true;
		vrWindow->width = width;
		vrWindow->height = height;
	}
}