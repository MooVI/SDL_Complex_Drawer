#version 120
attribute vec2 position;
attribute vec4 normal;
varying vec3 frag_normal;
varying vec3 frag_position;
uniform mat4 projection;
uniform mat4 modelview;
uniform sampler2D zValues;
uniform float time;

float rate =0.001;

void main(void) {

  vec4  graph_coord = vec4 (position, 0, 1);
  vec4 value = (texture2D(zValues, graph_coord.xy / 2.0 + 0.5));
  float sinT = sin(time*rate);
  float cosT = cos(time*rate);
  graph_coord.z= value.r*sinT +value.g*cosT;
  vec4 eye_position = modelview* vec4(position, graph_coord.z, 1);
  frag_position=eye_position.xyz;
  gl_Position = projection * eye_position;
  frag_normal = (modelview*vec4 (normal.wx*sinT+normal.yz*cosT,1.0,0.0)).xyz;

}
