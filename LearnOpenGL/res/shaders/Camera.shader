#shader vertex

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 texCoord;
void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	texCoord = aTexCoord;
}

#shader fragment

#version 330 core
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}