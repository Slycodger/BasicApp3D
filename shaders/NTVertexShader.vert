#version 460 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 color;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec4 inColor;

void main() {
	gl_Position = projection * view * transform * vec4(pos, 1);
	inColor = vec4(color, 1);
}