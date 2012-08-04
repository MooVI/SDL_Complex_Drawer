#version 120
attribute vec2 position;
varying vec4 graph_coord;
uniform mat4 textureTransform;
uniform mat4 vertexTransform;
uniform sampler2D mytexture;

void main(void) {

  graph_coord = textureTransform * vec4(position, 0, 1);
  graph_coord.z = (texture2D(mytexture, graph_coord.xy / 2.0 + 0.5).r);
  gl_Position = vertexTransform * vec4(position, graph_coord.z, 1);

}