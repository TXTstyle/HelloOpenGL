#include "Renderer.hpp"

#include <iostream>

void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
  printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
         "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
  printf("%s\n", message);
  exit(-1);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
       shader.Bind();
       va.Bind();
       ib.Bind();
       glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}