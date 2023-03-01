#include <core/window_frame.h>
#include <util/logging_system.h>
#include <util/formatted_exception.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>

WindowFrame::WindowFrame() :
	m_resizable(false), m_fullscreen(false), m_vsync(false), m_windowStruct(nullptr)
{}

WindowFrame::WindowFrame(std::string_view title, const glm::ivec2& size, bool resizable, bool fullscreen, bool vsync) :
	m_resizable(resizable), m_fullscreen(fullscreen), m_vsync(vsync), m_windowStruct(nullptr)
{
	// Setup the window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	// Create the window struct object
	m_windowStruct = fullscreen ? glfwCreateWindow(size.x, size.y, title.data(), glfwGetPrimaryMonitor(), nullptr) :
		glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);

	if (!m_windowStruct)
		throw FormattedException("Failed to create the GLFW window struct object.");

	glfwMakeContextCurrent(m_windowStruct);

	if (m_vsync)
		glfwSwapInterval(1);

	// Position the window at the centre of the monitor
	const GLFWvidmode* monitorInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_windowStruct, (monitorInfo->width - size.x) / 2, (monitorInfo->height - size.y) / 2);

	// Load the addresses for all the OpenGL functions
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		throw FormattedException("Failed to load the addresses for the OpenGL functions.");
}

WindowFrame::WindowFrame(WindowFrame&& temp) noexcept :
	m_windowStruct(temp.m_windowStruct), m_fullscreen(temp.m_fullscreen), m_resizable(temp.m_resizable), m_vsync(temp.m_vsync)
{
	temp.m_windowStruct = nullptr;
}

WindowFrame::~WindowFrame()
{
	if (m_windowStruct)
		glfwDestroyWindow(m_windowStruct);
}

WindowFrame& WindowFrame::operator=(WindowFrame&& temp) noexcept
{
	m_windowStruct = temp.m_windowStruct;
	m_fullscreen = temp.m_fullscreen;
	m_resizable = temp.m_resizable;
	m_vsync = temp.m_vsync;

	temp.m_windowStruct = nullptr;
	return *this;
}

void WindowFrame::SetPosition(const glm::ivec2& pos)
{
	glfwSetWindowPos(m_windowStruct, pos.x, pos.y);
}

void WindowFrame::SetSize(const glm::ivec2& size)
{
	glfwSetWindowSize(m_windowStruct, size.x, size.y);
}

void WindowFrame::RequestClose() const
{
	glfwSetWindowShouldClose(m_windowStruct, true);
}

void WindowFrame::Clear(const glm::vec3& fillColor)
{
	glClearColor(std::clamp(fillColor.r / 255.0f, 0.0f, 1.0f), std::clamp(fillColor.g / 255.0f, 0.0f, 1.0f), 
		std::clamp(fillColor.b / 255.0f, 0.0f, 1.0f), 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowFrame::Update() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_windowStruct);
}

bool WindowFrame::WasRequestedClose() const
{
	return glfwWindowShouldClose(m_windowStruct);
}

glm::ivec2 WindowFrame::GetPosition() const
{
	glm::ivec2 position = glm::ivec2(0);
	glfwGetWindowPos(m_windowStruct, &position.x, &position.y);
	return position;
}

glm::ivec2 WindowFrame::GetSize() const
{
	glm::ivec2 size = glm::ivec2(0);
	glfwGetWindowSize(m_windowStruct, &size.x, &size.y);
	return size;
}

std::string WindowFrame::GetGLFWVersion() const
{
	return glfwGetVersionString();
}

std::string WindowFrame::GetOpenGLVersion() const
{
	return (char*)glGetString(GL_VERSION);
}

bool WindowFrame::IsResizable() const
{
	return m_resizable;
}

bool WindowFrame::IsFullscreen() const
{
	return m_fullscreen;
}

bool WindowFrame::IsVSyncEnabled() const
{
	return m_vsync;
}

GLFWwindow* WindowFrame::GetFrameStruct() const
{
	return m_windowStruct;
}
