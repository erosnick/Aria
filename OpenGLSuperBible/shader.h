#ifndef SHADER_H
#define SHADER_H

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    Shader(): ID(0) {}

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvaluationPath = nullptr)
    {
        load(vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvaluationPath);
    }

    std::string loadShader(std::ifstream& file, const std::string& path)
    {
        file.open(path);
		std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();
		return shaderStream.str();
    }

    void load(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessControlPath = nullptr, const char* tessEvaluationPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
		std::string geometryCode;
		std::string tessControlCode;
		std::string tessEvaluationCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		std::ifstream tessControlShaderFile;
		std::ifstream tessEvaluationShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tessControlShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        tessEvaluationShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try 
        {
            // open files
			vertexCode = loadShader(vShaderFile, vertexPath);
            fragmentCode = loadShader(fShaderFile, fragmentPath);

            // if geometry shader path is present, also load a geometry shader
            if(geometryPath != nullptr)
            {
                geometryCode = loadShader(gShaderFile, geometryPath);
            }

			if (tessControlPath != nullptr)
			{
				tessControlCode = loadShader(tessControlShaderFile, tessControlPath);
			}

			if (tessEvaluationPath != nullptr)
			{
				tessEvaluationCode = loadShader(tessEvaluationShaderFile, tessEvaluationPath);
			}
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        // 2. compile shaders

        // vertex shader
        uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        uint32_t tessControl;
		if (tessControlPath != nullptr)
		{
			const char* tessControlShaderCode = tessControlCode.c_str();
            tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControl, 1, &tessControlShaderCode, nullptr);
			glCompileShader(tessControl);
			checkCompileErrors(tessControl, "TESS_CONTROL");
		}

        uint32_t tessEvaluation;
		if (tessEvaluationPath != nullptr)
		{
			const char* tessEvaluationShaderCode = tessEvaluationCode.c_str();
            tessEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvaluation, 1, &tessEvaluationShaderCode, nullptr);
			glCompileShader(tessEvaluation);
			checkCompileErrors(tessEvaluation, "TESS_EVALUTION");
		}

		// if geometry shader is given, compile geometry shader
		uint32_t geometry;
		if (geometryPath != nullptr)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}

		// fragment Shader
        uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

        // shader Program
        ID = glCreateProgram();

        glAttachShader(ID, vertex);

		if (tessControlPath != nullptr)
			glAttachShader(ID, tessControl);

		if (tessEvaluationPath != nullptr)
			glAttachShader(ID, tessEvaluation);

		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);

        glAttachShader(ID, fragment);

        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        if (tessControlPath != nullptr)
            glDeleteShader(tessControl);

        if (tessEvaluationPath != nullptr)
            glDeleteShader(tessEvaluation);

		if (geometryPath != nullptr)
			glDeleteShader(geometry);

    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif
