#version 310 es
#extension GL_EXT_shader_io_blocks : enable
#extension GL_OES_shader_io_blocks : enable

precision mediump float;
precision highp int;
precision lowp sampler2D;

in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}
