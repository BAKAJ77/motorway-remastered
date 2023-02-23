#ifndef WINDOW_FRAME_H
#define WINDOW_FRAME_H

#include <string_view>
#include <glm/glm.hpp>

struct GLFWwindow;

class WindowFrame
{
private:
	GLFWwindow* m_windowStruct;
	bool m_resizable, m_fullscreen, m_vsync;
public:
	WindowFrame();
	WindowFrame(std::string_view title, const glm::ivec2& size, bool resizable, bool fullscreen, bool vsync);
	WindowFrame(WindowFrame&& temp) noexcept;

	~WindowFrame();

	WindowFrame& operator=(WindowFrame&& temp) noexcept;

	// Sets the position of the window.
	void SetPosition(const glm::ivec2& pos);

	// Sets the size of the window.
	void SetSize(const glm::ivec2& size);

	// Requests for the window to be closed.
	void RequestClose() const;

	// Clears the window's back draw buffer.
	void Clear(const glm::vec3& fillColor);

	// Polls for events and swaps the draw buffers.
	void Update() const;
	
	// Returns TRUE if the window was requested to close.
	bool WasRequestedClose() const;

	// Returns the position of the window.
	glm::ivec2 GetPosition() const;

	// Returns the size of the window.
	glm::ivec2 GetSize() const;

	// Returns the version of the GLFW library being used.
	std::string GetGLFWVersion() const;

	// Returns the version of OpenGL being used.
	std::string GetOpenGLVersion() const;

	// Returns TRUE if the window is resizable.
	bool IsResizable() const;

	// Returns TRUE if the window is in fullscreen mode.
	bool IsFullscreen() const;

	// Returns TRUE if the window has VSync enabled.
	bool IsVSyncEnabled() const;
};

#endif