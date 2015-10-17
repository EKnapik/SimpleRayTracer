#version 120

attribute vec3 currVert;
attribute vec2 uvVert;
varying vec2 texCoord;

void main() {
    gl_Position = vec4(currVert, 1.0);
    texCoord = uvVert;
}