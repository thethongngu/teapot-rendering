#version 330 core

layout (location = 0) in vec3 in_vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 rotation_axis;
uniform float rotation_angle;

out vec3 out_vertex;

vec4 convert_to_quaternion(vec3 axis, float angle) {
    vec4 qr;
    float half_angle = (angle / 2) * 3.14159 / 180.0;
    qr.x = axis.x * sin(half_angle);
    qr.y = axis.y * sin(half_angle);
    qr.z = axis.z * sin(half_angle);
    qr.w = cos(half_angle);
    qr = normalize(qr);
    return qr;
}

vec3 rotate(vec3 pos, vec3 axis, float angle) {
    vec4 q = convert_to_quaternion(axis, angle);
    return pos + 2.0 * cross(q.xyz, cross(q.xyz, pos) + q.w * pos);
}

void main() {
    out_vertex = in_vertex;

    vec3 transformed = rotate(in_vertex + vec3(0, -0.5, 0), rotation_axis, rotation_angle);
    vec3 trans_transformed = rotate(transformed + vec3(5, 0, 0), rotation_axis, rotation_angle);
    gl_Position = projection * view * model * vec4(trans_transformed, 1.0);
}
