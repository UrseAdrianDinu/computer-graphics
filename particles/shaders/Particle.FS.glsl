#version 430

// Input
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in flat int f_color;

// Uniform properties
//uniform sampler2D texture_1;

uniform int mirrorred;

// Output
layout(location = 0) out vec4 out_color0;
layout(location = 1) out vec4 out_color1;
layout(location = 2) out vec4 out_color2;
layout(location = 3) out vec4 out_color3;
layout(location = 4) out vec4 out_color4;
layout(location = 5) out vec4 out_color5;



void main()
{
	if (mirrorred == 1) {
	  if (f_color == 0)
		out_color0 = vec4(vec3(1,0,0), 1);
	  if (f_color == 1)
		out_color1 = vec4(vec3(0,1,0), 1);
	  if (f_color == 2)
		out_color2 = vec4(vec3(0,0,1), 1);
	  if (f_color == 3)
		out_color3 = vec4(vec3(1,1,0), 1);
	  if (f_color == 4)
		out_color4 = vec4(vec3(1,0,1), 1);
	  if (f_color == 5)
		out_color5 = vec4(vec3(0,1,1), 1);
	} else {
		out_color0 = vec4(vec3(1,1,1), 1);
	}
}
