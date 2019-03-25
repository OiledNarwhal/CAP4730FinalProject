// default.vert

/*
  This simple GLSL vertex shader does exactly what 
  OpenGL would do -- It transforms the vertex positions
  using the default OpenGL transformation. It also passes
  through the texture coordinate, normal coordinate, and some
  other good stuff so we can use them in the fragment shader.
*/

// The input image we will be filtering in this kernel.
uniform sampler2D displacementTex;

uniform float displacementMapping;
uniform float TesselationDepth;

// This 'varying' vertex output can be read as an input
// by a fragment shader that makes the same declaration.
varying vec3 modelPos;
varying vec3 lightSourcePos;
varying vec3 normal;
varying vec2 texPos;

void main()
{
    // Render the shape using standard OpenGL position transforms.
    gl_Position = ftransform();

    // Copy the standard OpenGL texture coordinate to the output.
    texPos = gl_MultiTexCoord0.xy;
    
    normal = gl_Normal.xyz;
	modelPos = gl_Vertex.xyz;
    vec3 S=vec3(1,0,0);
    vec3 T=cross(S,normal);
    if(displacementMapping > 0.0){
        normal = normalize(normal);
        float delta_uv=1.0/TesselationDepth;
        float scale=0.5;
        float center=texture2D(displacementTex, texPos).x*scale;
        float hu_plus=texture2D(displacementTex, clamp(texPos+vec2(delta_uv,0),0.0,1.0)).x*scale;
        float hu_minus=texture2D(displacementTex, clamp(texPos+vec2(-delta_uv,0),0.0,1.0)).x*scale;
        float hv_plus=texture2D(displacementTex, clamp(texPos+vec2(0,delta_uv),0.0,1.0)).x*scale;
        float hv_minus=texture2D(displacementTex, clamp(texPos+vec2(0,-delta_uv),0.0,1.0)).x*scale;
        modelPos = modelPos + center*normal;
        normal=normal-S*(hu_plus-hu_minus)/(2.0*delta_uv)-T*(hv_plus-hv_minus)/(2.0*delta_uv);
    }//
    
    // Render the shape using modified position.
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix *  vec4(modelPos,1);
    
    // we may need this in the fragment shader...
    modelPos = (gl_ModelViewMatrix * vec4(modelPos,1)).xyz;

	// send the normal to the fragment shader
	normal = normalize((gl_NormalMatrix * normal).xyz);

    // pass the light source position to the fragment shader
    lightSourcePos = gl_LightSource[0].position.xyz;
}
