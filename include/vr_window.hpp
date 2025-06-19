#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//std
#include <string>

namespace vr
{
	class VrWindow
	{
	public:
		VrWindow(int w, int h, std::string n);
		~VrWindow();

		VrWindow(const VrWindow&) = delete;
		VrWindow& operator=(const VrWindow&) = delete;

		bool ShouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D GetExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool WasWindowResized() { return frameBufferResized; }
		void ResetWindowResizedFlag() { frameBufferResized = false; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void InitWindow();
		static void FrameBufferResizedCallback(GLFWwindow* window, int width, int height);

		int width, height;
		bool frameBufferResized;
		const std::string name;
		GLFWwindow* window;
	};
}