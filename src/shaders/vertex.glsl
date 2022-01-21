#version 330 core

layout(location = 0) in vec3 position;

out vec2 TexCoord;


void main() {

 mat4 model = mat4 (
 	1.0, 0.0, 0.0, 3.0, 
 	0.0, 1.0, 0.0, 0.0, 
 	0.0, 0.0, 1.0, 0.0, 
 	0.0, 0.0, 0.0, 1.0
 	);

 mat4 view = mat4 (
 	1.0, 0.0, 0.0, 0.0, 
 	0.0, 1.0, 0.0, 0.0, 
 	0.0, 0.0, 1.0, -3.0, 
 	0.0, 0.0, 0.0, 1.0
 	);

 mat4 projection = mat4 (
 	1.0, 0.0, 0.0, 0.0, 
 	0.0, 1.0, 0.0, 0.0, 
 	0.0, 0.0, 1.0, 0.0, 
 	0.0, 0.0, 0.0, 1.0
 	);

 gl_Position =  projection * view * model * vec4(position, 1);
 TexCoord = position.xy;
}
