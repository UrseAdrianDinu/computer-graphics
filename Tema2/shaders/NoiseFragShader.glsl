#version 330
// TODO: get values from fragment shader
in vec3 frag_normal;
in vec3 frag_color;
in vec3 frag_position;
in vec2 frag_coord;

layout(location = 0) out vec4 out_color;

uniform vec3 object_color;
uniform float Time;
uniform float rand;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform bool mixed;

void main()
{
	vec4 color = texture2D(texture_1, frag_coord);
	vec4 second_color = texture2D(texture_2, frag_coord);


	if (mixed == true) {
		out_color = mix(color, second_color, 0.5f);
	} else {
		out_color = color;
	}
}