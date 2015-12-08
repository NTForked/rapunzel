#version 440

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normals;

uniform mat4 model_matrix;
uniform mat4 view_matrix,projection_matrix;

out vec3 out_normals;

void main(){
	out_normals = in_normals;
	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1); 
}
