#version 330 core

uniform vec3 u_lightcolor;

void main()
{
	gl_FragColor = vec4(u_lightcolor, 1.0);
}
