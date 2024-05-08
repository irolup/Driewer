#version 330

#pragma include "lights.glsl"
#pragma include "material.glsl"

uniform sampler2DRect texture_diffuse;

in vec2 varyingtexcoord;
in vec4 v_vertex;
in vec3 v_normal;

out vec4 fragColor;

void main(void)
{
    vec2 uv = varyingtexcoord.xy;
    
    // Couleur diffuse du matériau
    vec3 flatColor = material.diffuse.rgb;
    
    // Couleur finale sans prise en compte de la lumière
    fragColor = vec4(flatColor, 1.0) * texture(texture_diffuse, uv);
}
