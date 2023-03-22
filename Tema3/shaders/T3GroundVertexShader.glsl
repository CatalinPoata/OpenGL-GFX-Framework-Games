#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec2 texcoord;
out vec3 world_position;
out vec3 world_normal;

void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    world_position = vec3(Model * vec4(v_position, 1));
    // interNormal = vec3(transpose(mat4(1, 0, 0, 0, 0, cos(radians(45.0)), -sin(radians(45.0)), 0, 0, sin(radians(45.0)), cos(radians(45.0)), 0, 0, 0, 0, 1)) * vec4(v_normal, 1));
    world_normal = vec3(Model * vec4(v_normal, 0));

    texcoord = v_texture_coord;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
