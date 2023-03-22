#version 330

// Input
// TODO(student): Get vertex attributes from each location

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 text_coord;
layout(location=3) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float scaleFactor;
uniform vec3 carPosition;

out vec3 frag_color;
out vec3 fnormal;
out vec2 fcoord;
out vec3 fposition;
out float ftime;

// Output
// TODO(student): Output values to fragment shader

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = color;
    // TODO(student): Compute gl_Position
    vec3 world_position = vec3(Model * vec4(position, 1));
    world_position.y = world_position.y - pow(length(carPosition - world_position), 2) * scaleFactor;
    gl_Position = Projection * View * vec4(world_position, 1);

}
