#version 150

in vec2 texCoord;
uniform sampler2D texture;

void main() {
    gl_FragColor = texure2D(texture, texCoord);
}