#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <ios>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void clearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

const int w = 800;
const int h = 600;

char *vshaderSource = new char[256]; // will point to the char array read from file
char *fshaderSource = new char[256];
// read_s will point source to the contents of fname which will be a shader program.
int read_s(const char *fname, char *source) {
	std::fstream fl;
	fl.open(fname);
	if (fl.bad()) {
		return -1;
	}
	auto i = 0;
	while (fl.good()) {
		source[i] = fl.get();
		if (!fl.eof()) {
			i++;
		}
	}
	std::cout << source << std::endl;
	fl.close();
	return 0;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(w, h, "OpenGL Crash Course", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, w, h);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f
	};
	unsigned int VBO; // vertex buffer object
	glGenBuffers(1, &VBO); // gen / register buffer id

	unsigned int VShader;
	VShader = { glCreateShader(GL_VERTEX_SHADER) };
	if (read_s("vertex_shader01.c", vshaderSource)==-1) {
		return -1;
	}
	glShaderSource(VShader, 1, &vshaderSource,nullptr);
	glCompileShader(VShader);

	int success;
	char info[512];
	glGetShaderiv(VShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(VShader, sizeof(info), nullptr, info);
		std::cout << "VShader failed:\n" << info << std::endl;
	} else { std::cout << "VShader passed." << std::endl; }

	unsigned int FShader;
	FShader = { glCreateShader(GL_FRAGMENT_SHADER) };
	if (read_s("fragment_shader01.c", fshaderSource) == -1) {
		return -1;
	}
	glShaderSource(FShader, 1, &fshaderSource, nullptr);
	glCompileShader(FShader);

	glGetShaderiv(FShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(FShader, sizeof(info), nullptr, info);
		std::cout << "FShader failed:\n" << info << std::endl;
	} else { std::cout << "FShader passed." << std::endl; }

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, VShader);
	glAttachShader(shaderProgram, FShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(info), nullptr, info);
		std::cout << info << std::endl;
	}
	glDeleteShader(VShader);
	glDeleteShader(FShader);
	glUseProgram(shaderProgram);

	unsigned int VAO; // vertex array object
	glGenVertexArrays(1, &VAO); // register one vertex array object
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // load vertices into buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);

	while (!glfwWindowShouldClose(window)) {
		clearColor(0.01f, 0.5f, 0.5f, 1.0f);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

