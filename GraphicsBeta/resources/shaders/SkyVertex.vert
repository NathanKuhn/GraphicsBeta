#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 out_pos;

uniform mat4 uTransform;

void main() {
	out_pos = (uTransform * vec4(aPos, 1.0)).xyz;
	gl_Position = vec4(aPos, 1.0);
}