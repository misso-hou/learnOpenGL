#version 330

out vec3 worldPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

const vec3 Pos[4] = vec3[4] (
    vec3(-1.0,0.0,-1.0),  //bottom left
    vec3(1.0,0.0,-1.0),   //bottom right
    vec3(1.0,0.0,1.0),    //top right
    vec3(-1.0,0.0,1.0)    //top left
);

const int Indices[6] = int[6](0,2,1,2,0,3);

void main()
{
    int Index = Indices[gl_VertexID];
    vec3 vPos3 = Pos[Index];
    vPos3.x += cameraPos.x;
    vPos3.z += cameraPos.z;

    vec4 vPos4 = vec4(vPos3,1.0);
    gl_Position = projection * view * vPos4;

    worldPos = vPos3;
}