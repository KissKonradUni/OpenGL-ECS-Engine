#include "shader.hpp"

unsigned int core::createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename)
{
    auto vertexShaderFile = file::readFileAsString(vertexShaderFilename);
    const char* vertexShaderSource = vertexShaderFile.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        DebugConsole::error(std::string("Vertex Shader Compilation Error: \n" + std::string(infoLog)).c_str());
    }

    auto fragmentShaderFile = file::readFileAsString(fragmentShaderFilename);
    const char* fragmentShaderSource = fragmentShaderFile.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //int success;
    //char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        DebugConsole::error(std::string("Fragment Shader Compilation Error: \n" + std::string(infoLog)).c_str());
    }

    unsigned int geometryShader = 0;
    if (geometryShaderFilename != nullptr) {
        auto geometryShaderFile = file::readFileAsString(geometryShaderFilename);
        const char* geometryShaderSource = geometryShaderFile.c_str();
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);

        //int success;
        //char infoLog[512];
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            DebugConsole::error(std::string("Geometry Shader Compilation Error: \n" + std::string(infoLog)).c_str());
        }
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    if (geometryShaderFilename != nullptr)
        glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //int success;
    //char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        DebugConsole::error(std::string("Shader Program Linking Error: \n" + std::string(infoLog)).c_str());
    }
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    if (geometryShaderFilename != nullptr)
        glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

namespace engine {
    glShader::glShader(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename)
    {
        this->mShaderIndex = core::createShader(vertexShaderFilename, fragmentShaderFilename, geometryShaderFilename);
        mUniformLocationCache = std::vector<std::pair<std::string, unsigned int>*>();
    }

    glShader::~glShader()
    {
        glDeleteProgram(mShaderIndex);
        mUniformLocationCache.clear();
        mUniformLocationCache.~vector();
    }

    void glShader::bind()
    {
        glUseProgram(mShaderIndex);
    }

    void glShader::setUniform(const char* name, bool value)
    {
        glUniform1i(getUniformLocation(name), (int)value);
    }

    void glShader::setUniform(const char* name, unsigned int value)
    {
        glUniform1ui(getUniformLocation(name), value);
    }

    void glShader::setUniform(const char* name, double value)
    {
        setUniform(name, (float)value);
    }

    void glShader::setUniform(const char* name, float value)
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void glShader::setUniform(const char* name, float x, float y)
    {
        glUniform2f(getUniformLocation(name), x, y);
    }

    void glShader::setUniform(const char* name, float x, float y, float z)
    {
        glUniform3f(getUniformLocation(name), x, y, z);
    }

    void glShader::setUniform(const char* name, float x, float y, float z, float w)
    {
        glUniform4f(getUniformLocation(name), x, y, z, w);
    }

    void glShader::setUniform(const char* name, glm::vec2 vector)
    {
        setUniform(name, vector.x, vector.y);
    }

    void glShader::setUniform(const char* name, glm::vec3 vector)
    {
        setUniform(name, vector.x, vector.y, vector.z);
    }

    void glShader::setUniform(const char* name, glm::vec4 vector)
    {
        setUniform(name, vector.x, vector.y, vector.z, vector.w);
    }

    void glShader::setUniform(const char* name, glm::mat4x4 matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    unsigned int glShader::getUniformLocation(std::string name)
    {
        std::string uniformID = std::to_string(this->mShaderIndex) + name;

        for (std::pair<std::string, unsigned int>* uniformCache : mUniformLocationCache) {
            if (uniformCache->first == uniformID)
                return uniformCache->second;
        }
        auto location = glGetUniformLocation(mShaderIndex, name.c_str());
        mUniformLocationCache.push_back(new std::pair<std::string, unsigned int>{uniformID, location});
        return location;
    }
}
