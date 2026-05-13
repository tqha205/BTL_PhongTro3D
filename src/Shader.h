#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::string paths[] = {"", "../", "../../", "../../../", "shaders/", "../shaders/", "../../shaders/"};
        bool success = false;
        for (const auto& p : paths) {
            try {
                // If the path already has "shaders/", don't prepend it again if p contains "shaders/"
                std::string vPath = p + vertexPath;
                std::string fPath = p + fragmentPath;
                vShaderFile.clear();
                fShaderFile.clear();
                vShaderFile.open(vPath);
                fShaderFile.open(fPath);
                if (vShaderFile.is_open() && fShaderFile.is_open()) {
                    std::stringstream vShaderStream, fShaderStream;
                    vShaderStream << vShaderFile.rdbuf();
                    fShaderStream << fShaderFile.rdbuf();
                    vShaderFile.close();
                    fShaderFile.close();
                    vertexCode = vShaderStream.str();
                    fragmentCode = fShaderStream.str();
                    success = true;
                    break;
                }
            } catch (...) {
                if (vShaderFile.is_open()) vShaderFile.close();
                if (fShaderFile.is_open()) fShaderFile.close();
            }
        }

        if (!success) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << vertexPath << " or " << fragmentPath << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    void use() { glUseProgram(ID); }
    void setBool(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
    void setInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
    void setFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
    void setVec3(const std::string &name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); }
    void setMat4(const std::string &name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
};
#endif
