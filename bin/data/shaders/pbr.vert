#version 330

// attributs de sommet
in vec4 position;
in vec4 normal;
in vec2 texcoord;

// attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;
out vec2 surface_texcoord;
out vec4 eyeSpaceVertexPos;
out vec3 interp_eyePos;
out vec2 varyingtexcoord;

// attributs uniformes
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

void main()
{
  // calculer la matrice normale
  mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));

  // transformation de la normale du sommet dans l'espace de vue
  surface_normal = vec3(normalMatrix * normal);

  // transformation de la position du sommet dans l'espace de vue
  surface_position = vec3(modelViewMatrix * position);

  eyeSpaceVertexPos = modelViewMatrix * position;
  interp_eyePos = vec3(-eyeSpaceVertexPos);

  // passer les coordonnées de textures au shader de fragment
  surface_texcoord = texcoord;
  varyingtexcoord = vec2(texcoord.x, texcoord.y);

  // transformation de la position du sommet par les matrices de modèle, vue et projection
  gl_Position = projectionMatrix * modelViewMatrix * position;
}
