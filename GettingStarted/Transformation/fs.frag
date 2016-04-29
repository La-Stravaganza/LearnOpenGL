#version 330 core
in vec3 vsOutColor;
in vec2 vsOutTextureCoordinate;

out vec4 fsOutColor;
uniform sampler2D uniformTexture1;
uniform sampler2D uniformTexture2;

void main() {
	fsOutColor = mix(texture(uniformTexture1, vsOutTextureCoordinate), texture(uniformTexture2, vsOutTextureCoordinate), 0.3);
}
