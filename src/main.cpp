#include <core/window_frame.h>
#include <core/shader_system.h>
#include <core/texture_system.h>
#include <core/input_system.h>

#include <graphics/vertex_array.h>
#include <graphics/camera_3d.h>

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
		
		LoggingSystem::GetInstance().Output("GLFW version: %s", LoggingSystem::Severity::INFO, applicationFrame.GetGLFWVersion().c_str());
		LoggingSystem::GetInstance().Output("OpenGL version: %s", LoggingSystem::Severity::INFO, applicationFrame.GetOpenGLVersion().c_str());

		// Initialize the input system
		InputSystem::GetInstance().SetFocusedWindow(applicationFrame);

		// Setup other objects here (TEMPORARY)
		ShaderSystem::GetInstance().Load("Geometry", { "shaders/common.glsl.vsh", "shaders/geometry.glsl.fsh" });
		TextureSystem::GetInstance().Load("Grass", "textures/test.jpg", false, false);

		float vertices[] = { 
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		};

		uint32_t indices[] = { 0, 1, 2, 1, 2, 3 };

		VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);
		vbo.PushLayout(0, GL_FLOAT, 3, 5 * sizeof(float));
		vbo.PushLayout(1, GL_FLOAT, 2, 5 * sizeof(float), 3 * sizeof(float));

		IndexBuffer ibo(indices, sizeof(indices), GL_STATIC_DRAW);

		VertexArray vao;
		vao.AttachBuffers(vbo, &ibo);

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

				accumulatedRenderTime -= timeStep;
			}

			// Render to screen and calculate the amount time taken to render
			const float preRenderTime = Time::GetSecondsSinceEpoch();
			
			//////// TEMPORARY ///////

			ShaderSystem::GetInstance().GetShader("Geometry").Bind();
			ShaderSystem::GetInstance().GetShader("Geometry").SetUniform("diffuseTexture", 0);
			ShaderSystem::GetInstance().GetShader("Geometry").SetUniform("useTexture", true);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, { 0.0f, 0.0f, -5.0f });
			model = glm::scale(model, { 1.2f, 1.2f, 1.0f });

			ShaderSystem::GetInstance().GetShader("Geometry").SetUniformEx("modelMatrix", model);
			ShaderSystem::GetInstance().GetShader("Geometry").SetUniformEx("cameraMatrix", camera.GetProjectionMatrix() * camera.GetViewMatrix());

			TextureSystem::GetInstance().GetTexture("Grass").Bind(0);
			vao.Bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			/////////////////////////

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
