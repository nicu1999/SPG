#version 430

layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

uniform sampler2D texture_1;
uniform samplerCube texture_cubemap;

uniform vec3 camera_position;

uniform int id;

uniform int highlight;

layout(location = 0) out vec4 out_color;

vec3 V;

vec3 myReflect()
{
    // TODO - compute the reflection color value
	vec3 R = reflect(-V, world_normal);
	return texture(texture_cubemap, R).xyz;
}

void main()
{
	V = normalize(camera_position - world_position);

    out_color = vec4(myReflect(), 0);

	if(highlight == 1){
		out_color += vec4(0, 1, 0, 0);
	} else {
		out_color += vec4(1, 0.1, 0.1, 0);
	}


	out_color.a = id / 255.0f;
}