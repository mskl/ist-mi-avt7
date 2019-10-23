#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;
uniform vec3 lightState;

uniform int[8] l_enabled;
uniform int[8] l_type;
uniform vec4[8] l_pos;

//           point    point    point    point    point    point    dir      spot
uniform vec4 l_pos_0, l_pos_1, l_pos_2, l_pos_3, l_pos_4, l_pos_5, l_pos_6, l_pos_7;
uniform vec4 l_spot_dir;

in vec4 position;
in vec4 normal;

out Data {
    vec3 normal;
    vec3 eye;
    vec3 lightDir[8];
} DataOut;

void main () {
    vec4 pos = m_viewModel * position;

    DataOut.normal = normalize(m_normal * normal.xyz);
    DataOut.eye = vec3(-pos);

    // Lights are off by default
    for (int i = 0; i < 8; i++) {
        DataOut.lightDir[i] = vec3(0);
    }

    // Point lights
    if(l_enabled[0] == 1)
        DataOut.lightDir[0] = vec3(l_pos_0 - pos);
    if(l_enabled[1] == 1)
        DataOut.lightDir[1] = vec3(l_pos_1 - pos);
    if(l_enabled[2] == 1)
        DataOut.lightDir[2] = vec3(l_pos_2 - pos);
    if(l_enabled[3] == 1)
        DataOut.lightDir[3] = vec3(l_pos_3 - pos);
    if(l_enabled[4] == 1)
        DataOut.lightDir[4] = vec3(l_pos_4 - pos);
    if(l_enabled[5] == 1)
        DataOut.lightDir[5] = vec3(l_pos_5 - pos);
    if(l_enabled[6] == 1) // directional light
        DataOut.lightDir[6] = vec3(l_pos_6);
    if(l_enabled[7] == 1) // cone light
        DataOut.lightDir[7] = vec3(l_pos_7 - pos);

    // Return the vertex position
    gl_Position = m_pvm * position;
}