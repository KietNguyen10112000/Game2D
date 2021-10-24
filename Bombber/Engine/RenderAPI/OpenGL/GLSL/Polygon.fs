#version 440 core

in vec2 ourUV;
in vec4 ourcolor;

// Ouput data
out vec4 color;

//info[0] == 1 => use texture, else use color
uniform vec4 info;

// Values that stay constant for the whole mesh.
uniform sampler2D colorTexture;

void main()
{
    // Output color = color of the texture at the specified UV
    if(info[0] == 1)
    {
        color = texture(colorTexture, ourUV);

        if(info[3] != 0)
        {
             color.w = info[3];
		}
	}  
    else
        color = ourcolor;
}