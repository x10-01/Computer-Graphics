#include<gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
/*
//处理每个顶点的着色器
const GLchar* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0f);\n"
"}";
//处理每个像素的着色器
const GLchar* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}";
static void CreateShaderProgram(GLuint& shaderProgram)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAIL\n" << infoLog << std::endl;
		exit(-1);
	}
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAIL\n" << infoLog << std::endl;
        exit(-1);
    }
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINK_FAIL\n" << infoLog << std::endl;
        exit(-1);
    }
#ifdef __WIN64
	glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::VALIDATE_FAIL\n" << infoLog << std::endl;
        exit(-1);
    }
#endif __WIN64
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
*/
//vertex specification
GLuint VAO, VBO;
static void CreateVertexBuffer()
{
	GLfloat vertices[] = 
    { //position           //color
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
static void SetViewport(GLFWwindow* window)
{
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
}
static void RenderObject(Shader &shader, GLuint &vao)
{
	//glUseProgram(shaderProgram);
    shader.UseProgram();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}


int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "B24070408", NULL, NULL);
    if (!window)
    {glfwTerminate(); return -1;}

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //init glew
	if (GLEW_OK != glewInit())
	{
		std::cout << "GLEW init fails!" << std::endl;
		glfwTerminate();exit(-1);
	}
    CreateVertexBuffer();
	//GLuint shaderProgram = glCreateProgram();
	//CreateShaderProgram(shaderProgram);
    /* Loop until the user closes the window */
    Shader shader = Shader("res/shaders/core.vs", "res/shaders/core.fs");
    while (!glfwWindowShouldClose(window))
    {
		SetViewport(window);
        /* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		//RenderObject(shaderProgram, VAO);
        RenderObject(shader, VAO);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}