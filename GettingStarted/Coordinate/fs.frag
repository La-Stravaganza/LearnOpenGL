#version 330 core
in vec2 vsOutTextureCoordinate;

out vec4 fsOutColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	//fsOutColor = mix(texture(texture1, vsOutTextureCoordinate), texture(texture2, vsOutTextureCoordinate), 0.3);
	fsOutColor = texture(texture2, vsOutTextureCoordinate);
}
