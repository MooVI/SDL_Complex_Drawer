#version 120
varying vec3 frag_normal;
varying vec3 frag_position;
struct Material {
 float shininess;
vec4 colour;
vec4 specular;
};

uniform Material material;

uniform mat4 projection;
uniform mat4 modelview;


const vec3 light_direction = vec3(0.408248, -0.816497, 0.408248);
const vec4 light_diffuse = vec4(0.8, 0.8, 0.8, 0.0);
const vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
const vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

void main(void) {
  vec3 mv_light_direction = (modelview * vec4(light_direction, 0.0)).xyz,
         normal = normalize(frag_normal),
         eye = normalize(frag_position),
         reflection = reflect(mv_light_direction, normal);

    vec4 diffuse_factor
        = max(-dot(normal, mv_light_direction), 0.0) * light_diffuse;
    vec4 ambient_diffuse_factor
        = diffuse_factor + light_ambient;
    vec4 specular_factor
        = max(pow(-dot(reflection, eye), material.shininess), 0.0)
            * light_specular;
    
    gl_FragColor = specular_factor * material.specular
        + ambient_diffuse_factor * material.colour;

}
