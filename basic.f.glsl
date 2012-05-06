#version 140 /* basic.f.glsl <-- f == fragment */

uniform float dt;

void main()
{
	gl_FragColor = vec4(sin(dt - 1.43),
			sin(dt), sin(dt + 1.43), 1);
}
