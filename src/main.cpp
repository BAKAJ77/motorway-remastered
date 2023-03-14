#include <core/window_frame.h>
#include <core/asset_system.h>
#include <core/input_system.h>
#include <core/player_actor.h>

#include <graphics/vertex_array.h>
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
		AssetSystem::GetInstance().LoadTexture("Container-Diffuse-Map", "textures/container_diffuse.png", false, false);
		AssetSystem::GetInstance().LoadTexture("Container-Specular-Map", "textures/container_specular.png", false, false);
		AssetSystem::GetInstance().LoadTexture("Container-Emission-Map", "textures/container_emission.png", false, false);

		PlayerActor playerActor({ -2.5f, 3.0f, 5.0f }, { 1600.0f, 900.0f }, { 0.0f, 0.0f, -1.0f }, true);

		// The main loop of the application
		constexpr float timeStep = 0.001f;
		float accumulatedRenderTime = 0.0f, elapsedRenderTime = 0.0f;

		while (!applicationFrame.WasRequestedClose())
		{
			// Update the application logic
			accumulatedRenderTime += elapsedRenderTime;
			while (accumulatedRenderTime >= timeStep)
			{
				if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_ESCAPE))
				{
					glfwTerminate();
					return EXIT_SUCCESS;
				}

				InputSystem::GetInstance().Update();
				playerActor.Update(timeStep);

				accumulatedRenderTime -= timeStep;
			}

			// Render to screen and calculate the amount time taken to render
			const float preRenderTime = Time::GetSecondsSinceEpoch();

			Renderer::GetInstance().Clear(Renderer::ClearFlag::COLOR_BUFFER_BIT | Renderer::ClearFlag::DEPTH_BUFFER_BIT, 
				{ 0.0f, 0.0f, 0.0f, 1.0f });
			
			//////// TEMPORARY ///////

			Geometry::Transform transform;
			transform.m_position = { 0.0f, -0.6f, -5.0f };
			transform.m_size = { 1.5f, 1.5f, 1.5f };

			Geometry::Material material;
			material.m_diffuseTexture = AssetSystem::GetInstance().GetTexture("Container-Diffuse-Map");
			material.m_specularTexture = AssetSystem::GetInstance().GetTexture("Container-Specular-Map");
			//material.m_emissionTexture = AssetSystem::GetInstance().GetTexture("Container-Emission-Map");
			material.m_enableTextures = true;

			SceneLighting lighting;
			Renderer::GetInstance().GetLighting().m_globalLight.m_ambientIntensity = { 0.2f, 0.2f, 0.2f };
			Renderer::GetInstance().GetLighting().m_globalLight.m_enabled = false;

			Renderer::GetInstance().GetLighting().m_pointLight.m_position = { 0.0f, 3.0f, 0.0f };
			Renderer::GetInstance().GetLighting().m_pointLight.m_enabled = true;

			Renderer::GetInstance().GetLighting().m_spotLight.m_position = playerActor.GetPosition();
			Renderer::GetInstance().GetLighting().m_spotLight.m_direction = playerActor.GetDirection();
			Renderer::GetInstance().GetLighting().m_spotLight.m_enabled = false;

			for (float x = -10.5f; x <= 10.5f; x += 1.5f)
			{
				for (float z = -10.5f; z <= 10.5f; z += 1.5f)
				{
					transform.m_position = { x, 0.0f, z };
					Renderer::GetInstance().Render(playerActor, Cube(transform, material), &lighting);
				}
			}

			transform.m_position = { 0.0f, 3.0f, 0.0f };
			transform.m_size = { 0.5f, 0.5f, 0.5f };

			material.m_diffuseColor = { 1.0f, 1.0f, 1.0f };
			material.m_enableTextures = false;

			Renderer::GetInstance().Render(playerActor, Cube(transform, material), nullptr);

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
