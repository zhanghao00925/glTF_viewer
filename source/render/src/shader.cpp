#include "shader.h"

Shader::Shader(const string &vertex, const string &fragment) {
    vertexPath = vertex;
    fragmentPath = fragment;
    Recompile();
}

void Shader::Recompile() {
//    Release();
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    vShaderFile.exceptions(ifstream::badbit);
    fShaderFile.exceptions(ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_ERROR" << endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "IN: " << vertexPath << " and " << fragmentPath << endl;
        cout << "ERROR::SHADER::VERTEX_COMPILE\n" << infoLog << endl;
    }
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "IN: " << vertexPath << " and " << fragmentPath << endl;
        cout << "ERROR::SHADER::FRAGMENT_COMPILE\n" << infoLog << endl;
    }glUseProgram(Program);
    Program = glCreateProgram();
    glAttachShader(Program, vertex);
    glAttachShader(Program, fragment);

    glLinkProgram(Program);
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Program, 512, NULL, infoLog);
        cout << "IN: " << vertexPath << " and " << fragmentPath << endl;
        cout << "ERROR::SHADER::PROGRAM_LINK\n" << infoLog<< endl;
    };

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const {
    glUseProgram(Program);
}

void Shader::Init(const string &vertex, const string &fragment) {
    vertexPath = vertex;
    fragmentPath = fragment;
    Recompile();
}
