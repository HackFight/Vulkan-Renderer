#pragma once

#include "vr_window.hpp"
#include "vr_pipeline.hpp"
#include "vr_device.hpp"
#include "vr_swap_chain.hpp"
#include "vr_model.hpp"

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
        HelloTriangleApp& operator=(const HelloTriangleApp&) = delete;

        void Run();


    private:
        void LoadModels();
        void CreatePipelineLayout();
        void RecreateSwapChain();
        void CreatePipeline();
        void CreateCommandBuffers();
        void FreeCommandBuffers();
        void DrawFrame();
        void RecordCommandBuffer(int imageIndex);

        VrWindow vrWindow{WIDTH, HEIGHT, "Hello Vulkan Triangle"};
        VrDevice vrDevice{ vrWindow };
        std::unique_ptr<VrSwapChain> vrSwapChain;
        std::unique_ptr<VrPipeline> vrPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<VrModel> vrModel;
	};
}