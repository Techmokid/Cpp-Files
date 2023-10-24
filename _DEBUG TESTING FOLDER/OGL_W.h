#ifndef OPENGL_CUSTOM_WRAPPER_H
#define OPENGL_CUSTOM_WRAPPER_H


// How to use:
//  - Use GetShaderCode(std::string path) to get a shader from a file instead of directly via code
//  - Use StartWindow() to begin. This is required to allow OpenGL to boot up and get everything running
//  - Use CompileShader(const char* shader) to compile the shader and get back it's corresponding GLuint memory ID.
//    For program performance, I recommend only calling this once at the start of your program, then simply saving it's
//    resulting GLuint ID somewhere to have a precompiled version of your shader ready to go
//  - Use StartShaderProgram(GLuint compiledShader) to start the shader up ready to be used
//  - Use InitializeShader() in order to skip needing to call GetShaderCode(), CompileShader(), and StartShaderProgram()


namespace OpenGL {
	std::vector<GLuint> shaderPrograms = std::vector<GLuint>();
	std::vector<GLuint> shaderShader = std::vector<GLuint>();
	
	
	
	
	
	
	
	
	
	
	
	
	
}

#endif