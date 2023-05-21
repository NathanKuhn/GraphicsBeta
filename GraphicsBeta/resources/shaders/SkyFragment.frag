#version 330 core

in vec3 out_pos;

out vec3 FragColor;

void main() {
	vec3 upColor = vec3(0.0, 0.71, 0.89);
	vec3 downColor = vec3(0.7, 0.87, 0.95);
	float fac = dot(normalize(out_pos), vec3(0, 1, 0));
	fac = clamp(fac * 2, 0.0, 2);
	FragColor = mix(downColor, upColor, fac);
}