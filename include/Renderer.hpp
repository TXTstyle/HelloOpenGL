#pragma once
#include <GL/glew.h>
#include <iostream>
#include "Debug.hpp"

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"



void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);

class Renderer
{
private:
    
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
