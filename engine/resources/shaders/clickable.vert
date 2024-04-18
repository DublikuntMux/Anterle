#version 310 es
#extension GL_EXT_shader_io_blocks : enable
#extension GL_OES_shader_io_blocks : enable

layout (location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}
