#version 330 core

layout(location = 0) in vec3 position;

out vec2 TexCoord;

uniform float angle;

void main() {

 mat4 model = mat4 (
 	1.0, 0.0, 0.0, 0.0, 
 	0.0, 1.0, 0.0, 0.0, 
 	0.0, 0.0, 1.0, 0.0, 
 	0.0, 0.0, 0.0, 1.0
 	);

 mat4 view = mat4 (
 	1.0, 0.0, 0.0, 0.0, 
 	0.0, 1.0, 0.0, 0.0, 
 	0.0, 0.0, 1.0, 0.0, 
 	0.0, -1.0, -5.0, 1.0
 	);


 mat4 rotation = mat4 (
 	cos(angle), 0.0, sin(angle), 0.0,
	0.0, 1.0, 0.0, 0.0, 
 	-sin(angle), 0.0, cos(angle), 0.0, 
 	0.0, 0.0, 0.0, 1.0
 	);


 float n = 0.1;
 float f = -100;

 const float PI_2 = 1.57079632679489661923;
 float scale = tan(90 * PI_2 /180) * n;

 float r = 1 * scale;
 float l = -r;

 float t = scale;
 float b = -t;

 mat4 projection = mat4 (
 	2 * n / ( r - l ), 0.0, 0.0, 0.0, 
 	0.0, 2 * n / ( t- b), 0.0, 0.0, 
 	(r + l) / (r -l ), (t + b)/ (t-b), -(f +n)/(f -n), -1, 
 	0.0, 0.0, - 2 * f * n / (f - n ), 0.0
 	);

gl_Position =  projection * view * rotation * model * vec4(position, 1);
 TexCoord = position.xy;
}
