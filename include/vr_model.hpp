#pragma once

#include "vr_device.hpp"

#define GLM_FORCE_RADIANS
#define FLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace vr
{
	class VrModel
	{
	public:
		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		VrModel(VrDevice& device, const std::vector<Vertex>& vertices);
		~VrModel();

		VrModel(const VrModel&) = delete;
		VrModel& operator=(const VrModel&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);


	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);

		VrDevice& vrDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}