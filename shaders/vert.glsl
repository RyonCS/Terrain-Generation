// ==================================================================
#version 330 core
layout(location=0) in vec3 position; 
layout(location=1) in vec2 texCoords;
layout(location=2) in vec3 normals;

// uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;
uniform mat4 u_ViewMatrix;

out vec4 color;
out vec2 v_texCoords;
out vec3 WorldPosition;
out vec3 v_normal;

void main()
{
    gl_Position = u_Projection * u_ViewMatrix * vec4(position, 1.0);

    color = vec4(position.y/400);

    v_texCoords = texCoords;

    WorldPosition = position;

    v_normal = normals;
}
