#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 Ke;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

// TODO(student): Declare any other uniforms
uniform int is_spot;
uniform float cut_Angle;
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{

    vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(-L, world_normal));

    // TODO(student): Define ambient, diffuse and specular light components
    vec3 ambient_light = 0.25 * Ka;
    vec3 diffuse_light =  Kd * max(dot(N, L), 0);
    vec3 specular_light = vec3(0, 0, 0);
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    
    if (diffuse_light.x > 0 )
    {
        specular_light = Ks * pow(max(dot(N, H), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    vec3 light = ambient_light;

    if (is_spot == 1) {
        float cut_off	= radians(cut_Angle);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        if (spot_light > spot_light_limit) {	 
            float linear_att		= (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
			float light_att_factor	= pow(linear_att, 2);
			light					= Ke + ambient_light + light_att_factor * (diffuse_light + specular_light);
        }
    
    }
    else
    {
        float dist	= distance(light_position, world_position);
		float factor_attenuation = 1 / pow(dist, 2);
	
		light	= Ke + ambient_light + factor_attenuation * (diffuse_light + specular_light);
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    vec3 colour = object_color * light;

    // TODO(student): Write pixel out color
    out_color = vec4(colour, 1.f);

}
