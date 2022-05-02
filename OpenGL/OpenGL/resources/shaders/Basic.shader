/*Vertex Shader Code*/
#shader vertex        //just for parsing //#shader not a syntax
#version 330 core     //use opengl version 330

layout(location = 0) in vec4 position;  //location=0 is the index of the attribute
void main()
{
 gl_Position = position;
};

/*Fragment Shader Code*/
#shader fragment      //just for parsing // #shader not a syntax
#version 330 core     //use opengl version 330

layout(location = 0) out vec4 color;  //location=0 is the index of the attribute
void main()
{
 color=vec4(1.0,0.0,0.0,1.0);
};