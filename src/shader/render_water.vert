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
    // y value (height of wave) -> function using x and z (OpenGL convention y = z height)
    // combine multiple sine waves
    float y_position = 0.0;

    y_position += 0.4 * sin(dot(aPos.xz, vec2(0.8, 0.3)) * frequency + time * speed);
    y_position += 0.3 * sin(dot(aPos.xz, vec2(-0.6, 0.9)) * frequency * 1.3 + time * speed * 1.1);
    y_position += 0.2 * sin(dot(aPos.xz, vec2(1.0, -0.4)) * frequency * 2.0 + time * speed * 1.7);
    y_position *= amplitude;

    vec3 newPos = vec3(aPos.x, y_position, aPos.z);
    fragPos = vec3(model * vec4(newPos, 1.0));

    // derivate x and y for normal vector
    float derivation_x = 0.0;
    float derivation_z = 0.0;

    derivation_x += 0.4 * frequency * cos(dot(aPos.xz, vec2(0.8, 0.3)) * frequency + time * speed) * 0.8;
    derivation_z += 0.4 * frequency * cos(dot(aPos.xz, vec2(0.8, 0.3)) * frequency + time * speed) * 0.3;
                                                    
    derivation_x += 0.3 * frequency * cos(dot(aPos.xz, vec2(-0.6, 0.9)) * frequency * 1.3 + time * speed * 1.1) * -0.6;
    derivation_z += 0.3 * frequency * cos(dot(aPos.xz, vec2(-0.6, 0.9)) * frequency * 1.3 + time * speed * 1.1) * 0.9;
                                                    
    derivation_x += 0.2 * frequency * cos(dot(aPos.xz, vec2(1.0, 0.4)) * frequency * 2.0 + time * speed * 1.7) * 1.0;
    derivation_z += 0.2 * frequency * cos(dot(aPos.xz, vec2(1.0, 0.4)) * frequency * 2.0 + time * speed * 1.7) * 0.4;

    vNormal = normalize(vec3(-derivation_x, 1.0, -derivation_z));

    vec4 viewPos = view * model * vec4(newPos, 1.0);
    waveHeight = y_position;

    gl_Position = proj * viewPos;
}
