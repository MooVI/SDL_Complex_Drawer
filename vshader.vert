#version 120
attribute vec2 position;
varying vec4 graph_coord;
uniform mat4 textureTransform;
uniform mat4 vertexTransform;
uniform sampler2D mytexture;
uniform float time;

float rate =0.001;

void main(void) {

  graph_coord = textureTransform * vec4(position, 0, 1);
  vec4 value = (texture2D(mytexture, graph_coord.xy / 2.0 + 0.5));
  graph_coord.z= value.r*sin(time*rate) +value.a*cos(time*rate);
  gl_Position = vertexTransform * vec4(position, graph_coord.z, 1);

}