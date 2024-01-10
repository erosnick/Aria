// OpenGLSuperBible.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <sb7.h>

#include "shader.h"

class OpenGLApp : public sb7::application
{
public:
	void init() override
	{
		sb7::application::init();

		info.windowWidth = 1280;
		info.windowHeight = 720;
	}

	void startup() override
	{
		//shader.load("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");

		shader.load("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag", nullptr, 
					"Assets/Shaders/shader.tesc", "Assets/Shaders/shader.tese");

		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glPointSize(5.0f);

		glPatchParameteri(GL_PATCH_VERTICES, 3);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(1, &VAO);
	}

	void render(double currentTime) override
	{
		static const GLfloat clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
		const GLfloat animatedColor[] = { (float)std::sin(currentTime) * 0.5f + 0.5f, 
								  (float)std::cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, clearColor);

		shader.use();

		GLfloat attrib[] = { (float)std::sin(currentTime) * 0.5f,
							 (float)std::cos(currentTime) * 0.6f,
							 0.0f, 0.0f };

		GLfloat color[] = { 0.0f, 0.8f, 1.0f, 1.0f };

		glVertexAttrib4fv(0, attrib);
		glVertexAttrib4fv(1, color);

		glDrawArrays(GL_PATCHES, 0, 3);
	}

protected:
	GLuint VAO = 0;
	Shader shader;
};

DECLARE_CONSOLE_MAIN(OpenGLApp);