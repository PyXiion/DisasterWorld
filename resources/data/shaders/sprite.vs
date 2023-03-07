#version 330 core
layout (location = 0) in vec4 vertex; // <vec 2 position, vec2 texCoord>

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  texCoords = vertex.zw;
  gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}