#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform bool mixed;
in vec2 texcoord;


layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color = texture2D(texture_1, texcoord);
	vec4 second_color = texture2D(texture_2, texcoord);

	if (mixed == true) {
		out_color = mix(color, second_color, 0.5f);
	} else {
		out_color = color;
	}
}