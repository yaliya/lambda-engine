 #version 330 core

in vec2 texcoords;
out vec4 finalColor;
            
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float scaleFactorX;
    float scaleFactorY;
    float shininess;
};

uniform vec4 mcolor;
uniform Material material;

void main()
{
	if(mcolor == vec4(0, 0, 0, 1))
	{
		finalColor = vec4(0, 1, 0, 1);
	}
	else
	{
		finalColor = vec4(mcolor + texture(material.diffuse, texcoords));
	}
}
