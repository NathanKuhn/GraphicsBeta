#version 330 core

in vec3 out_normal;
in vec2 out_texCoord;

uniform sampler2D uTexture;

out vec3 FragColor;

void main() {
	vec3 light = normalize(vec3(0.4, -1.0, 0.1));
	float lambert = clamp(dot(-light, out_normal) * 0.2, 0, 1);
	float fac = clamp(lambert + 1.0, 0, 1.5);
	FragColor = texture(uTexture, out_texCoord).xyz * fac;
}