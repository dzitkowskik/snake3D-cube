#version 150

uniform mat4 Projection;
uniform mat4 ModelView;

in vec3 position;
out vec3 vObjPos;

void main(void)
{
	vObjPos = position;
	gl_Position = Projection * ModelView * vec4(position,1.0);
}