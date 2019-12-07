uniform samplerCube cubemap;
varying vec3 worldPos;

void main() {

    vec4 color = textureCube(cubemap, worldPos);

    gl_FragColor = color;
}