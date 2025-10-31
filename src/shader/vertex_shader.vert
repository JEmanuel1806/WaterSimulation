#version 440 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float speed;
uniform float frequency;
uniform float time;
uniform float amplitude;

out float waveHeight;
out vec3 vNormal;
out vec3 fragPos; 

void main()
{
    // z value (height of wave) -> function using x and y
    float z_position = sin(aPos.x * frequency + time * speed) * amplitude       // x
                + cos(aPos.y * frequency + time * speed) * amplitude;           // y

    vec3 newPos = vec3(aPos.x, aPos.y, z_position);
    fragPos = vec3(model * vec4(newPos, 1.0));

    // derivate x and y for normal vector
    float derivation_x = amplitude * frequency * cos(aPos.x * frequency + time * speed);
    float derivation_y = -amplitude * frequency * sin(aPos.y * frequency + time * speed);

    vNormal = normalize(vec3(-derivation_x, -derivation_y, 1.0f));

    vec4 viewPos = view * model * vec4(newPos, 1.0);
    waveHeight = z_position;

    gl_Position = proj * viewPos;
}
