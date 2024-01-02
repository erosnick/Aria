// OpenGLSuperBible.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <sb7.h>

#include "shader.h"

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec2 padding;
};

struct Transform
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
};

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
		shader.load("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");

		shader.use();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, info.windowWidth, info.windowHeight);
	}

	void update(double currentTime) override
	{
	}

	void render(double currentTime) override
	{
		static const GLfloat clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
		static const float one = 1.0f;

		glClearBufferfv(GL_COLOR, 0, clearColor);
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

	void shutdown() override
	{
	}

	void onResize(int w, int h) override
	{
		sb7::application::onResize(w, h);

		glViewport(0, 0, info.windowWidth, info.windowHeight);
	}

protected:
	Shader shader;
	const float FrameTime = 0.016667f;
};

DECLARE_CONSOLE_MAIN(OpenGLApp);