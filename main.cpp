#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_XPOS 1230
#define WINDOW_YPOS 50

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		printf("pressed space at @ %.0f\n", glfwGetTime() * 1000);

	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		printf("released space at @ %.0f\n", glfwGetTime() * 1000);

	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	printf("mouse moved at @ %.0f: x=%f,y=%f\n", glfwGetTime() * 1000, xpos, ypos);
}

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW library\n");
		glfwTerminate();
		return -1;
	}

	GLFWwindow* window;
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game", nullptr, nullptr);
	if (!window)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, WINDOW_XPOS, WINDOW_YPOS);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glfwMakeContextCurrent(window);

	if (GLEW_OK != glewInit())
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	char* vertex_shader_source = nullptr;
	{
		size_t len;
		FILE* file = fopen("../identity.vert", "r");
		ssize_t bytes_read = getdelim(&vertex_shader_source, &len, '\0', file);
		if (bytes_read == -1)
		{
			fprintf(stderr, "Failed to read file\n");
		}
	}
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);
	{
		int shader_compiled;
		char info_log[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compiled);
		if (!shader_compiled)
		{
			glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
			fprintf(stderr, "Failed to compile shader\n%s", info_log);
		}
	}

	char* fragment_shader_source = nullptr;
	{
		size_t len;
		FILE* file = fopen("../yellow.frag", "r");
		ssize_t bytes_read = getdelim(&fragment_shader_source, &len, '\0', file);
		if (bytes_read == -1)
		{
			fprintf(stderr, "Failed to read file\n");
		}
	}
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);
	{
		int shader_compiled;
		char info_log[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compiled);
		if (!shader_compiled)
		{
			glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
			fprintf(stderr, "Failed to compile shader\n%s", info_log);
		}
	}

	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	{
		int shaders_linked;
		char info_log[512];
		glGetProgramiv(shader_program, GL_LINK_STATUS, &shaders_linked);
		if (!shaders_linked)
		{
			fprintf(stderr, "Failed to link shaders\n%s", info_log);
		}
	}
	glUseProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5, 0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}