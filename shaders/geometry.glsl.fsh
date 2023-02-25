#version 330 core

in VSH_OUT
{
    vec2 uvCoords;
} fshIn;

uniform vec4 diffuseColor;
uniform sampler2D diffuseTexture;
uniform bool useTexture;

void main()
{
    vec4 finalColor = diffuseColor;
    if (useTexture)
        finalColor = texture(diffuseTexture, fshIn.uvCoords);

    gl_FragColor = finalColor;
}
