#version 150

uniform mat4 Projection;
uniform mat4 ModelView;

in  vec3 vPosition;
in  vec3 vNormal;
out vec3 vObjPos;

void main(void)
{
	vObjPos = vPosition;
	gl_Position = Projection * ModelView * vec4(vPosition,1.0);
}