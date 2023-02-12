#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
0,1,2,
2,3,0
};

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"

"out vec4 vertexColor;\n"
"out vec2 TexCoord;\n"

"void main(){\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);\n"
"TexCoord = aTexCoord;\n"
"}\n";

const char* fragmentShaderSource =
"#version 330 core\n"

"in vec4 vertexColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"

"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = texture(ourTexture, TexCoord);\n"
"}\n";

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "我人有的和", NULL, NULL);
	if (window == NULL)
	{
		printf("开窗失败");
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW 初始化失败");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	


	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);





	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("图片加载失败");
	}
	stbi_image_free(data);



	while (!glfwWindowShouldClose(window))
	{
		processInput(window);


		glClearColor(1.0f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBindTexture(GL_TEXTURE_2D, TexBuffer);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glUseProgram(shaderProgram); 
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return 0;
}








