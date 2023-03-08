#ifndef RENDERER_H
#define RENDERER_H

#include <core/window_frame.h>
#include <core/shader_system.h>
#include <graphics/camera_3d.h>
#include <graphics/geometry.h>

class Renderer
{
private:
	Renderer() = default;
public:
	enum class ClearFlag : uint32_t
	{
		COLOR_BUFFER_BIT = 0x00004000,
		DEPTH_BUFFER_BIT = 0x00000100,
		STENCIL_BUFFER_BIT = 0x00000400
	};

	~Renderer() = default;

	// Initializes the rendering system.
	void Init() const;

	// Clears the current active framebuffer.
	void Clear(ClearFlag mask, const glm::vec4& color);

	// Renders the given geometry onto the scene of the currently active framebuffer.
	void Render(const Camera3D& camera, const Geometry& geometry) const;

	// Returns singleton instance of the class.
	static Renderer& GetInstance();
};

extern Renderer::ClearFlag operator|(const Renderer::ClearFlag& lhs, const Renderer::ClearFlag& rhs);

#endif