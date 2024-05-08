#version 330

#pragma include "lights.glsl"
#pragma include "material.glsl"

uniform sampler2DRect texture_diffuse;
uniform sampler2DRect texture_normal;

uniform int lightsNumber;

in vec2 varyingtexcoord;
in vec4 v_vertex;
in vec3 v_normal;

in vec4 lightDirections[8];

out vec4 fragColor;

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
    
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
    
    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal(vec3 N, vec3 V, vec2 texcoord)
{
    // Récupération de la valeur de la normal map
    vec3 map = texture(texture_normal, texcoord).xyz;
    
    // Transformation en coordonnées [-1, 1]
    map = map * 2.0 - 1.0;
    
    // Construction de la base tangente
    mat3 TBN = cotangent_frame(N, V, texcoord);
    
    // Perturbation de la normale
    return normalize(TBN * map);
}

void main(void)
{
    vec2 uv = varyingtexcoord.xy;
    vec3 totalDiffuse = vec3(0.0);
    
    vec3 N = normalize(v_normal);
    vec3 V = normalize(v_vertex.xyz);

    vec4 material_color;
    
    for (int i = 0; i < lightsNumber; i++)
    {
        vec3 L = normalize(lightDirections[i].xyz);
        
        // Perturbation de la normale par la normal map
        vec3 PN = perturb_normal(N, V, uv);
        
        // Calcul du terme de Lambert
        float lambertTerm = max(dot(PN, L), 0.0);
        
        // Calcul de la couleur diffuse
        vec3 diffuse = (lights.light[i].diffuse.rgb * material.diffuse.rgb) * lambertTerm;
        totalDiffuse += diffuse;

        material_color += material.ambient * material.emission * material.shininess * material.specular;
    }
    
    fragColor = vec4(totalDiffuse, 1.0) * texture(texture_diffuse, uv);
}
