// Pass 2 fragment shader
//
// Apply diffuse lighting to fragment.  Later do Phong lighting.
//
// Determine whether fragment is in shadow.  If so, reduce intensity to 50%.

#version 300 es
precision mediump float;


uniform vec3      lightDir;	        // direction to light in WCS
uniform vec3      eyePosition;	    // position of eye in WCS
uniform mat4      WCS_to_lightCCS;  // transform from WCS to light's CCS
uniform sampler2D shadowBuffer;     // texture [0,1]x[0,1] of depth from light.  Values in [0,1].
uniform sampler2D objTexture;       // object's texture (might not be provided)
uniform bool      texturing;        // =1 if object's texture is provided

in vec3 colour;        // fragment colour
in vec3 normal;        // fragment normal in WCS
in vec3 wcsPosition;   // fragemnt position in WCS
in vec2 texCoords;     // fragment texture coordinates (if provided) 

out vec4 fragColour;   // fragment's final colour

uniform vec3 ks;
uniform vec3 Ia;
uniform vec3 Ie;
uniform float shininess;
uniform vec3 Iin;

void main()

{
  // Calculate the position of this fragment in the light's CCS.

  vec4 ccsLightPos = WCS_to_lightCCS * vec4(wcsPosition, 1); // CHANGED

  // Calculate the depth of this fragment in the light's CCS in the range [0,1]
  
  float fragDepth = ((ccsLightPos.z/ccsLightPos.w)+1.0)/2.0;; // CHANGED

  // Determine the (x,y) coordinates of this fragment in the light's
  // CCS in the range [0,1]x[0,1].

  vec2 shadowTexCoords = vec2(((ccsLightPos.x/ccsLightPos.w)+1.0)/2.0,((ccsLightPos.y/ccsLightPos.w)+1.0)/2.0); // CHANGED

  // Look up the depth from the light in the shadowBuffer texture.

  float shadowDepth = texture(shadowBuffer, shadowTexCoords).x; // CHANGED

  // Determine whether the fragment is in shadow.
  //
  // If results look bad, add a bit to the shadow texture depth to
  // prevent z-fighting.
  // YOUR CODE HERE
  vec3 shadowColour = colour;
  if (fragDepth > shadowDepth+0.01)
  {
    shadowColour = shadowColour*0.3;
  }



  // Compute illumination.  Initially just do diffuse "N dot L".  Later do Phong.

  // DIFFUSE ILLUMINATION
  float NdotL = dot(normalize(lightDir),normalize(normal));
  if (NdotL < 0.0) NdotL = 0.0; 

  // PHONG ILLUMINATION
  vec3 R = 2.0 * NdotL * normal - lightDir;
  float RdotV = dot(normalize(eyePosition),normalize(R));

  vec3 phong = (shadowColour * NdotL * Iin + ks * Iin * pow((RdotV), shininess) + Ia + Ie);
  fragColour = vec4(phong,1.0);

  // Choose the colour either from the object's texture (if
  // 'texturing' == 1) or from the input colour.

  // YOUR CODE HERE

  // Output the fragment colour, modified by the illumination model
  // and shadowing.
  
  //fragColour = vec4(0.25,0.9,0.73,1);	// CHANGE THIS (used arbitrary )
}
