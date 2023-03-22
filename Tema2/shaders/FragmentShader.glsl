#version 330

// Input
// TODO(student): Get values from vertex shader

// Output
layout(location = 0) out vec4 out_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

in vec3 frag_color;
in vec3 fnormal;
in float ftime;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(frag_color, 1);
}
