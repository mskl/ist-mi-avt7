uniform samplerCube cubemap;
varying vec3 worldPos;

    void main() {

    worldPos = vec3(modelMatrix * vec4(position, 1.0));

    // Move position with camera
    gl_Position = projectionMatrix * viewMatrix * vec4(position + cameraPosition, 1.0);
}