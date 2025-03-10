#version 330

in vec3 worldPos;

layout(location = 0) out vec4 FragColor;

uniform float gGridCellSize = 0.025;
uniform vec4 gGridColorThin = vec4(0.5,0.5,0.5,1.0);
uniform vec4 gGridColorThick = vec4(0.0,0.0,0.0,1.0);

void main()
{

    float lod0a = mod(worldPos.z,gGridCellSize) / dFdy(worldPos.z );

    vec4 color = gGridColorThick;

    color.a *= lod0a;

    FragColor = color;
}