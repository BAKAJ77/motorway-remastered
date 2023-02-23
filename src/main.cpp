#include <core/window_frame.h>
#include <util/formatted_exception.h>
#include <util/logging_system.h>
#include <util/time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
	try
	{
		// Initialize the GLFW library
		if (!glfwInit())
			throw FormattedException("Failed to initialize the GLFW library");

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
	catch (std::exception& e)
	{
		FormattedException* formattedException = dynamic_cast<FormattedException*>(&e); // Check if the exception thrown is formatted

		if (formattedException)
			LoggingSystem::GetInstance().Output(formattedException->what(), LoggingSystem::Severity::FATAL, formattedException->GetArgs());
		else
			LoggingSystem::GetInstance().Output(e.what(), LoggingSystem::Severity::FATAL);

#ifdef _DEBUG
		std::cin.get();
#endif

		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}
