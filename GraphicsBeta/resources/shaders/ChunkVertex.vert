#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uTransform;

out vec3 out_normal;
out vec2 out_texCoord;

void main() {
	gl_Position = uTransform * vec4(aPos, 1.0);
	out_normal = aNormal;
	out_texCoord = aTexCoord;
}