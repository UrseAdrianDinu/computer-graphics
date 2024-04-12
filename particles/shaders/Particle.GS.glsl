#version 430

// Input and output topologies
layout(points) in;
layout(triangle_strip, max_vertices = 204) out;

// Uniform properties
uniform mat4 View0;
uniform mat4 View1;
uniform mat4 View2;
uniform mat4 View3;
uniform mat4 View4;
uniform mat4 View5;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;

uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
uniform int no_of_generated_points;
uniform float max_translate;
uniform float max_rotate;
uniform int mirrorred;

in int instance[1];
in vec4 g_color[1];
in vec4 angle[1];


// Output
layout(location = 0) out vec2 texture_coord;
layout(location = 1) out flat int f_color;


// Local variables
vec3 vpos = gl_in[0].gl_Position.xyz;



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

vec3 rotateZ(vec3 point, float u)
{
    float x = point.x * cos(u) - point.y *sin(u);
    float y = point.x * sin(u) + point.y *cos(u);
    return vec3(x, y, point.z);
}



vec3 bezier(float t)
{
    return  control_p0 * pow((1 - t), 3) +
            control_p1 * 3 * t * pow((1 - t), 2) +
            control_p2 * 3 * pow(t, 2) * (1 - t) +
            control_p3 * pow(t, 3);
}


void main() 
{

        float step = 1.0f /(no_of_generated_points);
        float t;
        float stepTranslate = 1.0f /(max_translate);
        float stepRotate = max_rotate / no_of_instances;

        vec4 rot_angle = angle[0];

        if (mirrorred == 1)
        {
            f_color = 0;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View0 * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View0 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View0 * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View0 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View0 * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View0 * vec4(vpos+ bezier_point3, 1);	EmitVertex();
                EndPrimitive();
   
            }

            f_color = 1;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View1 * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View1 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View1 * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View1 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View1 * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View1 * vec4(vpos+ bezier_point3, 1);	EmitVertex();
                EndPrimitive();
   
            }

            f_color = 2;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View2 * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View2 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View2 * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View2 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View2 * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View2 * vec4(vpos+ bezier_point3, 1);	EmitVertex();
                EndPrimitive();
   
            }

            f_color = 3;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View3 * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View3 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View3 * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View3 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View3 * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View3 * vec4(vpos+ bezier_point3, 1);	EmitVertex();
                EndPrimitive();
   
            }

            f_color = 4;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View4 * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View4 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View4 * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View4 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View4 * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View4 * vec4(vpos+ bezier_point3, 1);	EmitVertex();
                EndPrimitive();
   
            }

            f_color = 5;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View5 * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View5 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View5 * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View5 * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View5 * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View5 * vec4(vpos+ bezier_point3, 1);	EmitVertex();
                EndPrimitive();
   
            }
        }
        else 
        {
            f_color = 0;
            for(int i = 0 ; i < no_of_generated_points; i ++) {
                float curr = float(i) / no_of_generated_points;
                float next = float(i + 1) / no_of_generated_points;
                vec3 bezier_point1 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point2 = rotateZ(rotateY(rotateX(rotateY(bezier(curr) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point3 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, instance[0] * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                vec3 bezier_point4 = rotateZ(rotateY(rotateX(rotateY(bezier(next) * 0.25, (instance[0] + 1) * stepRotate), rot_angle.x), rot_angle.y), rot_angle.z);
                gl_Position = Projection * View * vec4(vpos + bezier_point1, 1);	EmitVertex();
                gl_Position = Projection * View * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View * vec4(vpos + bezier_point3, 1);	EmitVertex();
                EndPrimitive();

                gl_Position = Projection * View * vec4(vpos + bezier_point2, 1);	EmitVertex();
                gl_Position = Projection * View * vec4(vpos + bezier_point4, 1);	EmitVertex();
                gl_Position = Projection * View * vec4(vpos+ bezier_point3, 1);	    EmitVertex();
                EndPrimitive();
   
            }
        }

}
