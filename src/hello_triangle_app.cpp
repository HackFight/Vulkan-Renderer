#include "hello_triangle_app.hpp"

// std
#include <stdexcept>
#include <array>

namespace vr
{
	HelloTriangleApp::HelloTriangleApp()
	{
		LoadModels();
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}

	HelloTriangleApp::~HelloTriangleApp()
	{
		vkDestroyPipelineLayout(vrDevice.device(), pipelineLayout, nullptr);
	}

	void HelloTriangleApp::Run()
	{
		while (!vrWindow.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(vrDevice.device());
	}

	void HelloTriangleApp::LoadModels()
	{
		std::vector<VrModel::Vertex> vertices
		{
			{{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{-0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
			{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		vrModel = std::make_unique<VrModel>(vrDevice, vertices);
	}

	void HelloTriangleApp::CreatePipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(vrDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void HelloTriangleApp::CreatePipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		VrPipeline::DefaultPipelineConfigInfo( pipelineConfig, vrSwapChain.width(), vrSwapChain.height());
		pipelineConfig.renderPass = vrSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vrPipeline = std::make_unique<VrPipeline>(
			vrDevice,
			RESOURCES_PATH "shaders_bin/default_triangle.vert.spv",
			RESOURCES_PATH "shaders_bin/default_triangle.frag.spv",
			pipelineConfig);
	}

	void HelloTriangleApp::CreateCommandBuffers()
	{
		commandBuffers.resize(vrSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vrDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vrDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers!");
		}

		for (int i = 0; i < commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = vrSwapChain.getRenderPass();
			renderPassInfo.framebuffer = vrSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = vrSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.145f, 0.129f, 0.141f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vrPipeline->Bind(commandBuffers[i]);
			vrModel->Bind(commandBuffers[i]);
			vrModel->Draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to record command buffer!");
			}
		}
	}

	void HelloTriangleApp::DrawFrame()
	{
		uint32_t imageIndex;
		auto result = vrSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image");
		}

		result = vrSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!");
		}
	}
}