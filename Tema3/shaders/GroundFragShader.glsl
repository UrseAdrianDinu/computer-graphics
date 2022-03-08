#version 330

// Input
in vec3 world_position;
in vec3 world_normal;


uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 eye_position;

uniform vec3 Ke;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform vec3 spotlightpos[4];
uniform vec3 spotlightcolor[4];
uniform vec3 disco_ball_position;
uniform vec3 light_direction;
uniform float light_radius;
uniform float cut_Angle;
uniform vec3 object_color;

uniform int ground_light;
uniform int spot_light;
uniform int disco_light;

uniform float time;
uniform sampler2D texture1;

// Output
layout(location = 0) out vec4 out_color;

vec3 SpotLightContribution(vec3 lightPos, vec3 lightColor)
{
    vec3 color;
    vec3 N = normalize(world_normal);
	vec3 L = normalize(lightPos - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(-L, world_normal));

    // TODO(student): Define ambient, diffuse and specular light components
    vec3 diffuse_light =  lightColor * max(dot(N, L), 0);
    vec3 specular_light = vec3(0, 0, 0);
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    
    if (diffuse_light.x > 0 )
    {
        specular_light = lightColor * pow(max(dot(N, H), 0), material_shininess);
    }

     float cut_off	= radians(cut_Angle);
     float spot_light = dot(-L, light_direction);
     float spot_light_limit = cos(cut_off);
        if (spot_light > spot_light_limit) {
            float linear_att		= (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
			float light_att_factor	= pow(linear_att, 2);
			color					= light_att_factor * (diffuse_light + specular_light);
        }
    return color;
}

vec3 DiscoBallControbution()
{
    vec3 light_dir = world_position - disco_ball_position;
 
    // texcoord este coordonata de textura utilizata pentru esantionare
    vec2 texcoord;
    texcoord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) - time / 10.f;
    texcoord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
 
    // color este culoarea corespunzatoare pozitiei world_position
    vec3 color = texture2D(texture1, texcoord).xyz;


    vec3 N = normalize(world_normal);
	vec3 L = normalize(disco_ball_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(-L, world_normal));

    vec3 diffuse_light =  color * max(dot(N, L), 0);
    vec3 specular_light = vec3(0, 0, 0);
    
    if (diffuse_light.x > 0 )
    {
        specular_light = color * pow(max(dot(N, H), 0), material_shininess);
    }

     float dist	= distance(disco_ball_position, world_position);
     float factor_attenuation = 0.0f;
     if (dist < 6)
	    factor_attenuation = pow(6 - dist, 2);
	
	vec3 light_disco = color * factor_attenuation * (diffuse_light + specular_light);
    return light_disco;
}

void main()
{
    
    vec3 light = vec3(0);
    if(ground_light == 1){
        light += Ke;
        }

    if(spot_light == 1) {
        for(int i = 0; i < 4; i++)
        {
            light+=SpotLightContribution(spotlightpos[i],spotlightcolor[i]);
        }
    }

    if(disco_light == 1) {
        light+= DiscoBallControbution();
    }
   
   vec3 colour;
   if(ground_light == 0)
   {
        colour = light;
   }
   else
   {
        colour = object_color * light;
   }

    // TODO(student): Write pixel out color
    out_color = vec4(colour, 1.f);

}
