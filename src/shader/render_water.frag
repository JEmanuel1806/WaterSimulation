#version 440 core

out vec4 FragColor;

in float waveHeight;
in float fragDepth;
in vec3 fragPos;

// blinn phong 
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float reflectionFactor;

uniform samplerCube skybox;

in vec3 vNormal;

void main() {
    
    // direction vecs
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // a + d + s
    vec3 ambient = 0.2 * lightColor;
    vec3 diffuse = max(dot(vNormal, lightDir), 0.0) * lightColor;
    vec3 specular = 0.9 * pow(max(dot(vNormal, halfwayDir), 0.0), 256.0f) * lightColor;
    vec3 blinnPhong = (ambient + diffuse + specular) * objectColor;

    // reflection 
    vec3 viewDirEye = normalize(fragPos - viewPos);
    vec3 R = reflect(viewDirEye, normalize(vNormal));
    vec3 reflection = texture(skybox, R).rgb;

    float fresnel = pow(1.0 - max(dot(vNormal, -viewDirEye), 0.0), 3.0);  

    // final light
    vec3 color = mix(blinnPhong, reflection, fresnel * reflectionFactor);
    FragColor = vec4(color, 1.0);
}
