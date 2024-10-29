#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
            
out vec3 normals;
out vec2 texcoords;        
out vec3 fragmentPosition;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
    texcoords = vec2(texcoord.x, 1.0f - texcoord.y);
}