#version 330

uniform int texMode;
uniform sampler2D tex_road;
uniform sampler2D tex_river;
uniform sampler2D tex_grass;
uniform sampler2D tex_text;
uniform sampler2D tex_tree;
uniform sampler2D tex_particle;

struct Materials {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float shininess;
    int texcount;
};

// Output color from the shader
out vec4 colorOut;
uniform Materials mat;

// Stuff used for fog computation
in vec4 pos;
uniform int fogEnabled;

// Stuff used by lights
uniform vec4 l_spot_dir;

in Data {
    vec3 normal;
    vec3 eye;
    vec3 lightDir[8];
    vec2 tex_coord;
} DataIn;

vec3 getFog( in vec3 rgb, in float distance) {
    float fogAmount = exp(-distance*0.04);
    vec3 fogColor = vec3(0.5, 0.6, 0.7);
    return mix(fogColor, rgb, fogAmount);
}

void main() {
    #define a 0.3
    #define b 0
    #define c 0.04
    #define spotCutOff 0.9

    float intensity = 0.0f;
    vec4 spec = vec4(0.0);
    vec4 texel, texel1;

    // Calculate the lights output for all of the lights
    for (int i = 0; i < 8; i++) {
        vec3 n = normalize(DataIn.normal);
        vec3 l = normalize(DataIn.lightDir[i]);
        vec3 e = normalize(DataIn.eye);
        vec3 sd = normalize(-vec3(l_spot_dir));

        if (i <= 5) {
            /* POINTLIGHT */
            float distance = length(DataIn.lightDir[i]);
            float attenuation = 0.5/(a+(b*distance)+(c*distance*distance));
            intensity += max(dot(n,l), 0.0) * attenuation;
            if (intensity > 0.0) {
                vec3 h = normalize(l + e);
                float intSpec = max(dot(h,n), 0.0);
                spec += mat.specular * pow(intSpec, mat.shininess) * attenuation;
            }
        } else if (i == 6) {
            /* DIRLIGHT */
            intensity += max(dot(n,l), 0.0)*0.5;
            if (intensity > 0.0) {
                vec3 h = normalize(l + e);
                float intSpec = max(dot(h,n), 0.0);
                spec += mat.specular * pow(intSpec, mat.shininess)*0.5;
            }
        } else if (i == 7) {
            /* SPOTLIGHT */
            if(dot(l, sd) > 0.9)  {
                intensity += max(dot(n,l), 0.0);
                if (intensity > 0.0) {
                    vec3 h = normalize(l + e);
                    float intSpec = max(dot(h,n), 0.0);
                    spec += mat.specular * pow(intSpec, mat.shininess);
                }
            }
        }
    }

    if(mat.texcount == 1)
        texel = texture(tex_road, DataIn.tex_coord);
    else if(mat.texcount == 2)
        texel = texture(tex_river, DataIn.tex_coord);
    else if(mat.texcount == 3)
        texel = texture(tex_grass, DataIn.tex_coord);

    if (texMode == 0){
        // The plain color without texture
        colorOut = max((intensity * mat.diffuse + spec), mat.ambient);
        colorOut[3] = mat.diffuse[3];
    } else if(texMode == 2){
        colorOut = intensity*texel + spec;
        colorOut[3] = mat.diffuse[3];
    } else if(texMode == 3){ // Text
        vec4 cor = vec4(1,1,1,1);
        vec4 texColor = texture(tex_text, DataIn.tex_coord);
        if (texColor[0] + texColor[1] + texColor[2] < 2.5){
            discard;
        }
        colorOut = texColor*cor;
    } else if (texMode == 4){ // Tree
        vec4 texColor = texture(tex_tree, DataIn.tex_coord);
        if (texColor[3] == 0.0) {
            discard;
        }
        colorOut = intensity*texColor + spec;
        colorOut[3] = texColor[3];
    } else if (texMode == 5) { // Particle
        vec4 texColor = texture(tex_particle, DataIn.tex_coord);
        if (texColor[3] == 0.0 || (mat.diffuse[3] == 0.0)) {
            discard;
        }
        colorOut = mat.diffuse * texColor;
        colorOut[3] = texColor[3];
    }

    // Mix the fog with the final color of the fragment
    if (fogEnabled == 1) {
        float dist = length(pos);
        vec3 fogged = getFog(colorOut.rgb, dist);
        colorOut = vec4(fogged, colorOut[3]);
    }
}