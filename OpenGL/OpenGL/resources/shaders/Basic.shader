/*Vertex Shader Code*/
#shader vertex        //just for parsing //#shader not a syntax
#version 330 core     //use opengl version 330

layout(location = 0) in vec4 position;  //location=0 is the index of the attribute //attribute = 0 : position
layout(location = 1) in vec2 texCoord;  //attribute = 1 : texture

out vec2 v_TexCoord; //Pass data to fragment shader

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
//Projection View Model Matrix
//projection matrix * view matrix * model matrix * vector3

 gl_Position = u_Projection * u_View * u_Model * position;

 v_TexCoord = texCoord;
};

/*Fragment Shader Code*/
#shader fragment      //just for parsing // #shader not a syntax
#version 330 core     //use opengl version 330

in vec2 v_TexCoord; //Read data from vertex shader

//passing data from cpu using uniform
uniform vec4 u_Color; 
uniform float u_size;
uniform sampler2D u_Texture; //texture slot

layout(location = 0) out vec4 color;  //location=0 is the index of the attribute

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};