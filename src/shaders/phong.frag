#version 330

out vec4 colorOut;

struct Materials {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float shininess;
    int texCount;
};

uniform Materials mat;
uniform vec4 l_spot_dir;

in Data {
    vec3 normal;
    vec3 eye;
    vec3 lightDir[8];
} DataIn;

in vec4 pos; // Used for fog computation
uniform int fogEnabled;

vec3 getFog( in vec3 rgb, in float distance) {
    float fogAmount =  exp(-distance*0.04);
    vec3 fogColor = vec3(1, 1, 1);
    return mix(fogColor, rgb, fogAmount);
}

void main() {
    #define a 0.3
    #define b 0
    #define c 0.04
    #define spotCutOff 0.9

    float intensity = 0.0f;
    vec4 spec = vec4(0.0);

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

    colorOut = max((intensity * mat.diffuse + spec), mat.ambient);

    // Mix the fog with the final color of the fragment
    if (fogEnabled == 1) {
        float dist = length(pos);
        vec3 fogged = getFog(colorOut.rgb, dist);
        colorOut = vec4( fogged , 1);
    }
}