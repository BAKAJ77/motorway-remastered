#include <core/window_frame.h>
#include <core/shader_system.h>
#include <core/texture_system.h>
#include <core/input_system.h>

#include <graphics/vertex_array.h>
#include <graphics/camera_3d.h>
#include <graphics/renderer.h>

#include <util/formatted_exception.h>
#include <util/logging_system.h>
#include <util/time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
	try
	{
		// Initialize the GLFW library
		if (!glfwInit())
			throw FormattedException("Failed to initialize the GLFW library");

		// Create the application window
		WindowFrame applicationFrame("Motorway Remastered", { 1600, 900 }, false, false, false);
		applicationFrame.SetCursorMode(false);
		applicationFrame.SetContextActive();
		
		LoggingSystem::GetInstance().Output("GLFW version: %s", LoggingSystem::Severity::INFO, applicationFrame.GetGLFWVersion().c_str());
		LoggingSystem::GetInstance().Output("OpenGL version: %s", LoggingSystem::Severity::INFO, applicationFrame.GetOpenGLVersion().c_str());
		
		// Initialize the rendering and input system
		Renderer::GetInstance().Init();
		InputSystem::GetInstance().SetFocusedWindow(applicationFrame);

		// Setup other objects here (TEMPORARY)
		TextureSystem::GetInstance().Load("Grass", "textures/test.jpg", false, false);
		Camera3D camera({ 0.0f, 0.0f, 0.0f }, { 1600.0f, 900.0f });

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
				InputSystem::GetInstance().Update();
				camera.Update();

				if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_ESCAPE))
				{
					glfwTerminate();
					return EXIT_SUCCESS;
				}

				accumulatedRenderTime -= timeStep;
			}

			// Render to screen and calculate the amount time taken to render
			const float preRenderTime = Time::GetSecondsSinceEpoch();

			Renderer::GetInstance().Clear(Renderer::ClearFlag::COLOR_BUFFER_BIT | Renderer::ClearFlag::DEPTH_BUFFER_BIT, 
				{ 0.0f, 0.0f, 0.0f, 1.0f });
			
			//////// TEMPORARY ///////

			Geometry::Transform transform;
			transform.m_position = { 0.0f, 0.0f, -5.0f };
			transform.m_size = { 1.2f, 1.2f, 1.0f };

			Geometry::Material material;
			material.m_diffuseTexture = TextureSystem::GetInstance().GetTexture("Grass");
			material.m_enableTextures = true;

			Renderer::GetInstance().Render(camera, Square(transform, material));

			/////////////////////////

			applicationFrame.Update();

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
