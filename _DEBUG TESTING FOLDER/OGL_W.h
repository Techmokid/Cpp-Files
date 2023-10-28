#ifndef OPENGL_CUSTOM_WRAPPER_H
#define OPENGL_CUSTOM_WRAPPER_H

// -I"D:\OpenGL Libraries\freeglut\include"
// -L"D:\OpenGL Libraries\freeglut\lib\x64"
// -I"D:\OpenGL Libraries\glew-2.1.0\include"
// -L"D:\OpenGL Libraries\glew-2.1.0\lib\Release\x64"
// -I"D:\OpenGL Libraries\glfw-3.3.8.bin.WIN64\include"
// -L"D:\OpenGL Libraries\glfw-3.3.8.bin.WIN64\lib-mingw-w64"


// Include statements
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <filesystem>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

namespace OpenGL {
	// Variable definition
	std::vector<GLuint> shaderPrograms = std::vector<GLuint>();
	std::vector<GLuint> shaderShader = std::vector<GLuint>();
	
	
	// Function Definitions
	std::string GetShaderFromDisk(std::string shaderPath) {
		// Get the compute shader from disk
		std::ifstream myfile;
		myfile.open(shaderPath);
	  
		if (myfile.fail()) {
			throw std::invalid_argument("OpenGL file could not retreive filedata from file that does not exist: " + shaderPath);
		}
	  
		std::string computeShaderSourceString;
	  
		char c;
		while(myfile.get(c)) {
			computeShaderSourceString += c;
		}
		myfile.close();
		return computeShaderSourceString;
	}
	
	static void error_callback(int error, const char* description) {
		std::string descriptionString(description);
		throw std::invalid_argument("Internal Error: " + descriptionString + "\n");
  
		switch (error) {
		case GLFW_NOT_INITIALIZED:
			throw std::invalid_argument("GLFW has not been initialized.\n");
			break;
		case GLFW_NO_CURRENT_CONTEXT:
			throw std::invalid_argument("No current OpenGL or OpenGL ES context.\n");
			break;
		case GLFW_INVALID_ENUM:
			throw std::invalid_argument("Invalid enum value passed as argument.\n");
			break;
		case GLFW_INVALID_VALUE:
			throw std::invalid_argument("Invalid value passed as argument.\n");
			break;
		case GLFW_OUT_OF_MEMORY:
			throw std::invalid_argument("Out of memory.\n");
			break;
		case GLFW_API_UNAVAILABLE:
			throw std::invalid_argument("Requested OpenGL or OpenGL ES version is unavailable.\n");
			break;
		case GLFW_VERSION_UNAVAILABLE:
			throw std::invalid_argument("Requested OpenGL or OpenGL ES version is not supported by the graphics driver.\n");
			break;
		case GLFW_PLATFORM_ERROR:
			throw std::invalid_argument("A platform-specific error occurred.\n");
			break;
		case GLFW_FORMAT_UNAVAILABLE:
			throw std::invalid_argument("Requested pixel format or framebuffer configuration is not available.\n");
			break;
		default:
			throw std::invalid_argument("Unknown error.\n");
			break;
		}
	}
	
	void StartWindow(bool hideWindow) { StartWindow(hideWindow, 1, 1); }
	void StartWindow(int w, int h) { StartWindow(false,w,h); }
	void StartWindow(bool hideWindow, int w, int h) {
		glfwInit();
		if (!glfwInit()) {
			throw std::invalid_argument("glfwInit() failed to start");
		}
		
		glfwSetErrorCallback(error_callback);
		if (hideWindow)
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Optional
		
		GLFWwindow* window = glfwCreateWindow(w, h, "OpenGL Compute-shader Window", NULL, NULL);
		if (!window) {
			glfwTerminate();
			throw std::invalid_argument("glfwCreateWindow() failed to start");
		}

		glfwMakeContextCurrent(window);
		glewExperimental = GL_TRUE;
		  
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw std::invalid_argument("glewInit() failed to start");
		}
	}
	
	GLuint CompileShader(std::string computeShaderCode) {
		char* cstr = new char[computeShaderCode.length() + 1];
		std::strcpy(cstr, computeShaderCode.c_str());
		return CompileShader(cstr);
	}
	GLuint CompileShader(const char* computeShaderSource) {
		GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(computeShader, 1, &computeShaderSource, NULL);
		glCompileShader(computeShader);
		checkShaderCompileStatus(computeShader);
		
		bool addVal = true;
		for (int i = 0; i < shaderShader.size(); i++) {
			if (shaderShader[i] == computeShader)
				addVal = false;
		}
		
		if (addVal)
			shaderShader.push_back(computeShader);
  
		return computeShader;
	}
	
	GLuint StartShaderProgram(GLuint computeShader) {
		GLuint computeProgram = glCreateProgram();
		glAttachShader(computeProgram, computeShader);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
		glLinkProgram(computeProgram);
		glUseProgram(computeProgram);
		
		shaderPrograms.push_back(computeProgram);
		
		return computeProgram;
	}
	
	GLuint InitializeShader(std::string shaderPath) {
		std::string computeShaderCode = GetShaderCode(shaderPath);
		std::string computeShaderCodeCopy = computeShaderCode;
		computeShaderCodeCopy = RemoveSpecificCharacter(computeShaderCodeCopy,' ');
		computeShaderCodeCopy = RemoveSpecificCharacter(computeShaderCodeCopy,'\t');
		computeShaderCodeCopy = RemoveSpecificCharacter(computeShaderCodeCopy,'\n');
		computeShaderCodeCopy = RemoveSpecificCharacter(computeShaderCodeCopy,'\r');
		if (computeShaderCodeCopy.length() == 0)
			printFormatted("OpenGL", "Warning", "Compute shader is empty: " + shaderPath);
		
		//const char* versionStr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
		//std::cout << "GLSL version supported by the current context: " << versionStr << std::endl;
		//std::cout << "OpenGL version supported by the current context: " << glGetString(GL_VERSION) << std::endl;
		
		char* cstr = new char[computeShaderCode.length() + 1];
		std::strcpy(cstr, computeShaderCode.c_str());
		
		GLuint computeHandle = CompileShader(cstr);
		
		StartShaderProgram(computeHandle);
		
		return computeHandle;
	}

	void checkShaderCompileStatus(GLuint shader) {
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			char log[1024];
			GLsizei length;
			glGetShaderInfoLog(shader, sizeof(log), &length, log);
			printFormatted("OpenGL", "Error", "Shader compilation failed: " + std::string(log));
			quit();
		}
	}

	void ShutDownOpenGL() {
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		
		while (shaderPrograms.size() != 0) {
			glDeleteProgram(shaderPrograms[0]);
			shaderPrograms.erase(shaderPrograms.begin());
		}
		while (shaderShader.size() != 0) {
			glDeleteShader(shaderShader[0]);
			shaderShader.erase(shaderShader.begin());
		}
		
		//while (ssbos.size() != 0) {
		//  glDeleteBuffers(1, &ssbos[0]);
		//  ssbos.erase(ssbos.begin());
		//}
		
		glfwTerminate();
	}
	
	template<typename T>
	void Set_SSBO_Buffer(std::vector<T> &obj, GLuint ssbo, int buffer_index) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, obj.size() * sizeof(T), obj.data(), GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer_index, ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Set_SSBO_Buffer(T &obj, GLuint ssbo, int buffer_index) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &obj, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer_index, ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Set_SSBO_Buffer(T obj, GLuint ssbo, int buffer_index) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T), &obj, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer_index, ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Get_SSBO_Buffer(std::vector<T> &obj, GLuint ssbo) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, obj.size() * sizeof(T), obj.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Get_SSBO_Buffer(T &obj, GLuint ssbo) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(T), obj.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Edit_SSBO_Buffer(std::vector<T> &obj, GLuint ssbo) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, obj.size() * sizeof(T), obj.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Edit_SSBO_Buffer(T &obj, GLuint ssbo) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, obj.size() * sizeof(T), obj.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void Edit_SSBO_Buffer(T obj, GLuint ssbo) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(T), &obj);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

#endif