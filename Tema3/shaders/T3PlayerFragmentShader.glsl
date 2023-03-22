#version 330

// Inputsd
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_t;
uniform vec3 lightPositions[21];
uniform vec3 eye_position;
uniform vec3 objColor;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int lightTypes[21];
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color = vec4(objColor, 1);
    vec3 ambient_light = vec3(0.25, 0.25, 0.25);
    vec3 diffuse_light = vec3(0, 0, 0);
    vec3 specular_light = vec3(0, 0, 0);
    float primesteLumina = 0;
    float factorAtenuare  = 0;

    vec3 norm_world_normal = normalize(world_normal);

    ambient_light = ambient_light * material_kd;

    vec3 sum = ambient_light;

    for(int i = 0; i < 21; i++){
        primesteLumina = 0;
        factorAtenuare = 0;
        vec3 L = (lightTypes[i] == 0) ? vec3(0, 1, 0) : normalize(lightPositions[i] - world_position);
        vec3 V = normalize(eye_position - world_position);
        vec3 H = normalize(L + V);
        vec3 culoare_lumina;
        if(lightTypes[i] == 0 || lightTypes[i] == 1){
            culoare_lumina = vec3(1, 1, 1);
        }
        else if(lightTypes[i] == 2){
            culoare_lumina = vec3(0, 1, 0);
        }
        else if(lightTypes[i] == 3){
            culoare_lumina = vec3(1, 1, 0);
        }

        diffuse_light = material_kd * culoare_lumina * max(dot(norm_world_normal, L), 0);
        if(diffuse_light.x > 0 || diffuse_light.y > 0 || diffuse_light.z > 0){
            primesteLumina = 1;
        }

        specular_light = material_ks * culoare_lumina * primesteLumina * pow(max(dot(norm_world_normal, H), 0), material_shininess);
        float cutoff = radians(7.0);

        float spotlight = dot(-L, vec3(0, -0.72, -0.72));
        if(lightTypes[i] == 1){
            vec3 L1 = normalize(lightPositions[i] + vec3(1, 3, 2) - world_position);
            vec3 L2 = normalize(lightPositions[i] + vec3(-1, 3, 2) - world_position);
            vec3 H1 = normalize(L1 + V);
            vec3 H2 = normalize(L2 + V);
            vec3 diffuse_light1 = material_kd / 100 * culoare_lumina * max(dot(norm_world_normal, L1), 0);
            vec3 diffuse_light2 = material_kd / 100 * culoare_lumina * max(dot(norm_world_normal, L2), 0);
            int primesteLumina1 = 0;
            int primesteLumina2 = 0;

            if(diffuse_light1.x > 0 && diffuse_light1.y > 0 && diffuse_light1.z > 0){
                primesteLumina1 = 1;
            }

            if(diffuse_light2.x > 0 && diffuse_light2.y > 0 && diffuse_light2.z > 0){
                primesteLumina2 = 1;
            }

            float spotlight1 = dot(-L1, vec3(0, -0.72, -0.72));
            float spotlight2 = dot(-L2, vec3(0, -0.72, -0.72));

            vec3 specular_light1 = material_ks / 100 * culoare_lumina * primesteLumina1 * pow(max(dot(norm_world_normal, H1), 0), material_shininess);
            vec3 specular_light2 = material_ks / 100 * culoare_lumina * primesteLumina2 * pow(max(dot(norm_world_normal, H2), 0), material_shininess);
            float factorAtenuare1 = 0;
            float factorAtenuare2 = 0;

            if(spotlight1 > cos(cutoff)){
                float linearATT = (spotlight1 - cos(cutoff)) / (1.0f - cos(cutoff));
                factorAtenuare1 = pow(linearATT, 10);
            }
            else{
                factorAtenuare1 = 0;
            }

            if(spotlight2 > cos(cutoff)){
                float linearATT = (spotlight2 - cos(cutoff)) / (1.0f - cos(cutoff));
                factorAtenuare2 = pow(linearATT, 10);
            }
            else{
                factorAtenuare2 = 0;
            }

            sum += (diffuse_light1 + specular_light1) * factorAtenuare1 + (diffuse_light2 + specular_light2) * factorAtenuare2;

        }else{
            if(lightTypes[i] == 0){
                factorAtenuare = 1;
            }
            else{
                factorAtenuare = 1 / (pow(distance(lightPositions[i], world_position), 2) + 1);
            }
             sum += (diffuse_light + specular_light) * factorAtenuare;
        }
    }
     
    out_color = vec4(sum, 0) * color;
    if(color.w < 0.1f){
        discard;
    }

}
