#version 430

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform int speed_mod;
uniform int stop;
uniform int reset;

// Output
out int instance;
out vec4 g_color;
out vec4 angle;


struct Particle
{
    vec4 position;
    vec4 speed;
    vec4 iposition;
    vec4 ispeed;
    vec4 color;
    vec4 direction;
    vec4 angle;
    vec4 rotDirection;
};


layout(std430, binding = 0) buffer particles {
    Particle data[];
};


float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}

vec3 rotateX(vec3 point, float u)
{
    float y = point.y * cos(u) - point.z *sin(u);
    float z = point.y * sin(u) + point.z *cos(u);
    return vec3(point.x, y, z);
}

vec3 translate(vec3 point, vec3 t)
{
    return vec3(point.x + t.x, point.y + t.y, point.z + t.z);
}


void main()
{
    vec3 pos = data[gl_VertexID].position.xyz;
    vec3 spd = data[gl_VertexID].speed.xyz;
    vec3 dir = data[gl_VertexID].direction.xyz;
    vec3 rotDir = data[gl_VertexID].rotDirection.xyz;

    float dt = 0.1;

    if (stop == 0) {
        pos = pos + spd * dir * dt + dir * dt * dt/2.0f;
        spd = spd + speed_mod * 0.0001;
        if(rotDir.x == 1)
            data[gl_VertexID].angle += 0.01f;
        else
            data[gl_VertexID].angle -= 0.01f;
    }

    if (pos.y < 0 || pos.y > 19 || pos.x < 0 || pos.x > 19 || pos.z < 0 || pos.z > 19 || reset == 1)
    {
        pos = data[gl_VertexID].iposition.xyz;
        spd = data[gl_VertexID].ispeed.xyz;
    }

    data[gl_VertexID].position.xyz = pos;
    data[gl_VertexID].speed.xyz =  spd;
    
    g_color = data[gl_VertexID].color;
    angle = data[gl_VertexID].angle;
    instance = gl_InstanceID;
    gl_Position =  Model  * vec4(pos, 1) ;
}
