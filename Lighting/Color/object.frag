#version 330 core
out vec4 fsOutColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
	fsOutColor = vec4(lightColor * objectColor, 1.0f);
}
