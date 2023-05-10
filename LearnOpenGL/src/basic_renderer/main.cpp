#include "Render.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"
#include "Shader.h"

#include <iostream>


static int Width = 800;
static int Height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processinput(GLFWwindow* window);

int main(int argc, char argv[])
{
	if (!glfwInit())
	{
		std::cout << "GLFW initialize failed!\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(Width, Height, "Learn OpenGL", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Window create failed!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialize failed!\n";
		glfwTerminate();
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float position[] = {
		-0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};
	unsigned int indices[] = {
		0,1,2,
		2,3,1
	};

	VertexBuffer vb(position, sizeof(position));
	vb.Bind();
	IndexBuffer ib(indices, 6);
	ib.Bind();
	VertexLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);

	Shader shader("res/shaders/Basic.shader");

	shader.Bind();
	while (!glfwWindowShouldClose(window))
	{
		processinput(window);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		ib.Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader.Unbind();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processinput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}