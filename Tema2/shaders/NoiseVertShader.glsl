#version 330

layout (location =0) in vec3 v_position;
layout (location =1) in vec3 v_normal; 
layout (location =2) in vec2 v_text_coord; 
layout (location =3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 texcoord;
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_coord;

uniform float Time;
uniform float rand;

// Random number
float rand3D(in vec3 co){
    return fract(sin(dot(co.xyz ,vec3(15.5413,68.233,122.7312))) * 43758.5453);
}

// Perlin noise 3D
float simple_interpolate(in float a, in float b, in float x)
{
   return a + smoothstep(0.0,1.0,x) * (b-a);
}

float interpolatedNoise3D(in float x, in float y, in float z)
{
    float integer_x = x - fract(x);
    float fractional_x = x - integer_x;

    float integer_y = y - fract(y);
    float fractional_y = y - integer_y;

    float integer_z = z - fract(z);
    float fractional_z = z - integer_z;

    float v1 = rand3D(vec3(integer_x, integer_y, integer_z));
    float v2 = rand3D(vec3(integer_x+1.0, integer_y, integer_z));
    float v3 = rand3D(vec3(integer_x, integer_y+1.0, integer_z));
    float v4 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z));

    float v5 = rand3D(vec3(integer_x, integer_y, integer_z+1.0));
    float v6 = rand3D(vec3(integer_x+1.0, integer_y, integer_z+1.0));
    float v7 = rand3D(vec3(integer_x, integer_y+1.0, integer_z+1.0));
    float v8 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z+1.0));

    float i1 = simple_interpolate(v1,v5, fractional_z);
    float i2 = simple_interpolate(v2,v6, fractional_z);
    float i3 = simple_interpolate(v3,v7, fractional_z);
    float i4 = simple_interpolate(v4,v8, fractional_z);

    float ii1 = simple_interpolate(i1,i2,fractional_x);
    float ii2 = simple_interpolate(i3,i4,fractional_x);

    return simple_interpolate(ii1 , ii2 , fractional_y);
}

float Noise3D(in vec3 coord, in float wavelength)
{
   return interpolatedNoise3D(coord.x/wavelength, coord.y/wavelength, coord.z/wavelength);
}


void main()
{
	frag_position = v_position;
	frag_coord = v_text_coord;
	frag_normal = v_normal;
	frag_color = v_color;

	float randVal = rand3D(v_position)*15;
	vec3 newPos = v_position + v_normal  * sin(Time * randVal)*5;
	gl_Position = Projection * View * Model * vec4(newPos, 1.0);
}