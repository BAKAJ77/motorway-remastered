#include <core/window_frame.h>
#include <util/logging_system.h>
#include <util/time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	try
	{
		// Initialize the GLFW library
		if (!glfwInit())
			LoggingSystem::GetInstance().Output("Failed to initialize the GLFW library.", LoggingSystem::Severity::FATAL);

		// Create the application window
		WindowFrame applicationFrame("Motorway Remastered", { 1600, 900 }, false, false, false);
		
		LoggingSystem::GetInstance().Output("GLFW version: %s", LoggingSystem::Severity::INFO, applicationFrame.GetGLFWVersion().c_str());
		LoggingSystem::GetInstance().Output("OpenGL version: %s", LoggingSystem::Severity::INFO, applicationFrame.GetOpenGLVersion().c_str());

		// The main loop of the application
		constexpr float timeStep = 0.001f;
		float accumulatedRenderTime = 0.0f, elapsedRenderTime = 0.0f;

		while (!applicationFrame.WasRequestedClose())
		{
			// Update the application logic
			accumulatedRenderTime += elapsedRenderTime;
			while (accumulatedRenderTime >= timeStep)
			{
				// *The Update function should be here* //

				accumulatedRenderTime -= timeStep;
			}

			// Render to screen and calculate the amount time taken to render
			const float preRenderTime = Time::GetSecondsSinceEpoch();
			
			// *The Render function should be here* //

			applicationFrame.Update();
			applicationFrame.Clear({ 0, 0, 0 });

			const float postRenderTime = Time::GetSecondsSinceEpoch();
			elapsedRenderTime = postRenderTime - preRenderTime;
		}
	}
	catch (int& exceptionCode)
	{
		return exceptionCode;
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}
