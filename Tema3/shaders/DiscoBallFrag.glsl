#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 eye_position;

uniform vec3 Ke;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform vec3 light_direction;
uniform float light_radius;
uniform float cut_Angle;
uniform vec3 object_color;
uniform vec3 disco_ball_position;

uniform sampler2D texture1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{

    out_color = vec4(texture2D(texture1, texcoord).xyz, 1.0f);

}
