#pragma once

#include "vr_window.hpp"
#include "vr_pipeline.hpp"
#include "vr_device.hpp"
#include "vr_swap_chain.hpp"

// std
#include <memory>
#include <vector>

namespace vr
{
	class HelloTriangleApp
	{
    public:
        static constexpr int WIDTH = 800, HEIGHT = 600;

        HelloTriangleApp();
        ~HelloTriangleApp();

        HelloTriangleApp(const HelloTriangleApp&) = delete;
        void operator=(const HelloTriangleApp&) = delete;

        void Run();


    private:
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffers();
        void DrawFrame();

        VrWindow vrWindow{WIDTH, HEIGHT, "Hello Vulkan Triangle"};
        VrDevice vrDevice{ vrWindow };
        VrSwapChain vrSwapChain{ vrDevice, vrWindow.GetExtent() };
        std::unique_ptr<VrPipeline> vrPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
	};
}