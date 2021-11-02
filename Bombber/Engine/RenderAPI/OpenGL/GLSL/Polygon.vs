#version 440 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

out vec2 ourUV;
out vec4 ourcolor;

uniform vec4 screenDim;

uniform vec4 transform;

uniform mat3 polygonTransform;

uniform vec4 info;

void main()
{
    vec2 transformedPos = (vec3(position, 1.0f) * polygonTransform).xy;

    if(transform[0] != 0)
    {
        //transform[0] is angle
        float radAngle = transform[0];// "-" - clockwise
        float _x = transformedPos.x;
        float _y = transformedPos.y;

        vec2 pivot = vec2(transform[1], transform[2]);

        float rX = pivot.x + (_x - pivot.x) * cos(radAngle) - (_y - pivot.y) * sin(radAngle);
        float rY = pivot.y + (_x - pivot.x) * sin(radAngle) + (_y - pivot.y) * cos(radAngle);

        transformedPos = vec2(rX, rY);
    }
    

    float x = transformedPos.x / screenDim.x;
    float y = transformedPos.y / screenDim.y;

    vec2 pos =  vec2(2.0f * x - 1.0f, 1.0f - 2.0f * y);

    gl_Position = vec4(pos, 0, 1.0f);

    ourUV = uv;

    if (info[1] == 1)
    {
        ourUV.y = 1.0f - ourUV.y;
    }
    else if(info[1] == 2)
    {
        ourUV.x = 1.0f - ourUV.x;
    }

    //x = ourUV.x;
    //ourUV.x = -ourUV.y;
    //ourUV.y = x;

    ourcolor = color;
}