#include "basic_renderer/shader.h"
#include "vendor/stb_image.h"

#include <iostream>



//Window parameter
const int Width = 800;
const int Height = 600;


//Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//Rotation parameter
static bool firstMouse = true;
float pitch = 0.0f;
float yaw = -90.0f;
float lastXpos = (float)Width / 2;
float lastYpos = (float)Height / 2;
float fov = 45.0f;

//Timing
float deltaTime = 0.0f;
float lastTime = 0.0f;

//设置窗口回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//获取键盘信息
void processinput(GLFWwindow* window);
//获取鼠标信息
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//获取滚轮信息，yoffset代表滚轮滚动，不要忘了在透视矩阵中使用fov
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cout << "GLFW init failed!\n";
		glfwTerminate();
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
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD load failed!\n";
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	/*unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vertex, nullptr);
	glShaderSource(fs, 1, &fragment, nullptr);
	glCompileShader(vs);
	glCompileShader(fs);

	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);
	glValidateProgram(shader);
	glDetachShader(shader, vs);
	glDetachShader(shader, fs);*/

	Shader shader("res/shaders/Camera.shader");

	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		//position			//texture coordinate
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	/*更多的立方体，每个立方体都有一个位置*/
	//在渲染循环里循环画出这10个立方体

	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,   0.0f,   0.0f ),
		glm::vec3( 2.0f,   5.0f,  -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f ),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3( 2.4f,  -0.4f,  -3.5f ),
		glm::vec3(-1.7f,   3.0f,  -7.5f ),
		glm::vec3( 1.5f,   2.0f,  -2.5f ),
		glm::vec3( 1.5f,   0.2f,  -1.5f ),
		glm::vec3( 1.3f,  -2.0f,  -2.5f ),
		glm::vec3(-1.3f,   1.0f,  -1.5f )
	};


	/*设置旋转矩阵*/

	/*
	--------------------------------------------------------------------------------

	glm旋转和位移都是利用四元数来进行的，但是实际传参时我们只需要将位移和旋转轴的 3维 向量传入即可
	在四元数的应用时，分量 x y z w 其中若 w = 1，则向量经矩阵可以转换到相应空间；

	--------------------------------------------------------------------------------
	*/

	//glm::mat4 trans = glm::mat4(1.0f);											//初始化为单位矩阵
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));	//绕3D轴z逆时针旋转90度(PI/2)
	//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));							//将x、y、z方向缩小为原来的一半

	/*glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));*/



	unsigned int vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	unsigned int va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	/**************************************************************/
	/**************************************************************/
	/**************************************************************/
	//注意修改GLSL里的内容，包括Location位置，维度等等
	/**************************************************************/
	/**************************************************************/
	/**************************************************************/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/**************************************************************/
	/**************************************************************/
	/**************************************************************/


	unsigned int tex1;
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	/*设置纹理坐标、环绕方式和过滤选项*/
	//纹理坐标是从 (0.0)->(1.0,1.0)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*加载纹理图片，创建纹理*/
	int width, height, nrchannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/textures/container.jpg", &width, &height, &nrchannels, 0);
	//导入图片数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//生成多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	//释放内存
	stbi_image_free(data);

	unsigned int tex2;
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("res/textures/awesomeface.png", &width, &height, &nrchannels, 0);
	/*
	--------------------------------------------------------------------------
									PNG格式的图片在加载时采用 RGBA 格式加载
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	//glUseProgram(shader);
	shader.Bind();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);
	shader.Unbind();



	/*			配置摄像机方向等属性
	*
	* 该摄像机系统没有横滚、旋转属性

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);		//初始化位置
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);	//摄像机目标坐标
	glm::vec3 cameraDirction = glm::normalize(cameraPos - cameraTarget);		//摄像机面向目标的反方向，因为要满足向量叉乘（右手系坐标）
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);									//始终指向世界坐标上方

	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirction));		//因为摄像机方向始终指向背面，所以叉乘出来的向量始终指向摄像机的右方
																				//但是当摄像机位置处于世界坐标Z轴时失效

	glm::vec3 cameraUp = glm::cross(cameraDirction, cameraRight);				//利用摄像机方向向量和摄像机有向量叉乘得出摄像机上向量，始终指向摄像机的正上方


	*/





	shader.Bind();
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;


		processinput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2);


		//	一些模型、视角等上的设置，包括旋转、平移、伸缩

		/*-------------------------模型变换----------------------------*/
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(50 * (float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 0.0f));
		//shader.SetMatrix4fv("model", glm::value_ptr(model));

		/*-----------------------观察视角变换--------------------------*/

		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//shader.SetMatrix4fv("view", glm::value_ptr(view));

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.SetMatrix4fv("view", glm::value_ptr(view));

		/*--------------------------透视投影----------------------------*/
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float)Width / (float)Height, 0.1f, 100.0f);
		shader.SetMatrix4fv("projection", glm::value_ptr(projection));

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i = 0; i < 10; i++)
		{
			/*-----------------------模型变换-----------------------------*/
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, currentTime * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMatrix4fv("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader.Unbind();

	glDeleteBuffers(1, &vb);
	glDeleteVertexArrays(1, &va);

	/*glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(shader);*/
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
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastXpos = (float)Width / 2;
		lastYpos = (float)Height / 2;
		firstMouse = false;
	}
	float xoffset = xpos - lastXpos;
	float yoffset = lastYpos - ypos;	//反过来减，在角度变化的时候相加
	lastXpos = xpos;
	lastYpos = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;			//镜头右移，x坐标就往左移，所以偏航角角度增大
	pitch += yoffset;		//镜头往上移，y坐标就往下移，所以俯仰角角度减小

	/*一些判断用来防止镜头偏转过度*/
	if (pitch >= 89.0f)
		pitch = 89.0f;
	if (pitch <= -89.0f)
		pitch = -89.0f;

	/*旋转之后设置摄像头的前向量*/
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}