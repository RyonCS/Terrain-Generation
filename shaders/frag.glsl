#version 330 core

// The final output color of each 'fragment' from our fragment shader.

in vec4 color;
in vec2 v_texCoords;
in vec3 WorldPosition;
in vec3 v_normal;

out vec4 FragColor;
// out vec4 HeightColor;

uniform sampler2D waterTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

uniform float waterHeight = 3.5;
uniform float grassHeight = 50.0;
uniform float rockHeight = 75.0;
uniform float snowHeight = 350.0;

uniform vec3 u_ReversedLightDir;

vec4 CalculateTextureColor() {
    vec4 TexColor;

    float Height = WorldPosition.y;

    // Add water texture.
    if (Height < waterHeight) {
        TexColor = texture(waterTexture, v_texCoords);

    // Blend rock into grass.
    } else if (Height < grassHeight) {
        float blendFactor = smoothstep(waterHeight, grassHeight, Height);
        vec4 rockColor = texture(rockTexture, v_texCoords);
        vec4 grassColor = texture(grassTexture, v_texCoords);
        TexColor = mix(rockColor, grassColor, blendFactor);

    // Blend grass into rock.
    } else if (Height < rockHeight) {
        vec4 Color0 = texture(grassTexture, v_texCoords);
        vec4 Color1 = texture(rockTexture, v_texCoords);
        float Delta = rockHeight - grassHeight;
        float Factor = (Height - grassHeight) / Delta;
        TexColor = mix(Color0, Color1, Factor);

    // Blend rock into snow.
    } else if (Height < snowHeight) {
        vec4 Color0 = texture(rockTexture, v_texCoords);
        vec4 Color1 = texture(snowTexture, v_texCoords);
        float Delta = snowHeight - rockHeight;
        float Factor = (Height - rockHeight) / Delta;
        TexColor = mix(Color0, Color1, Factor);
    
    // Add snow texture.
    } else {
        TexColor = texture(snowTexture, v_texCoords);
    }

    return TexColor;
}
void main()
{
    vec4 texColor = CalculateTextureColor();

    vec3 v_normal = normalize(v_normal);

    float ambientStrength = 0.75f;
    vec4 ambient = ambientStrength * vec4(1.0);

    float diffuse = dot(v_normal, u_ReversedLightDir);

    diffuse = max(0.5f, diffuse);

    FragColor = ambient * diffuse * texColor;

}