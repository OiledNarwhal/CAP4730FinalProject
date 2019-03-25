// phong.frag

/*
  This fragment implements the Phong Reflection model.
*/

// The input image we will be filtering in this kernel.
uniform sampler2D normalTex;
uniform sampler2D colorTex;

uniform float normalMapping;
uniform float colorMapping;

varying vec3 modelPos;    // fragment position in model space
varying vec2 texPos;      // fragment position in texture space
varying vec3 lightSourcePos; // light source position in model space
varying vec3 normal;	  // fragment normal in model space

void main()
{
    // Sample from the normal map, if we're not doing displacement mapping
    vec3 N;
	if (normalMapping > 0.0){
        // convert a normal map to fit our plane
        vec3 temp=(2.*texture2D(normalTex, texPos).xyz - 1.);
        N = gl_NormalMatrix * vec3(temp.y,temp.z,temp.x);

        // use this code for regular normal mapping
        //N = gl_NormalMatrix * (2.*texture2D(normalTex, texPos).xyz - 1.);
    }
	else
		N = normal;
    
    N=normalize(N);
    
    vec3 C = vec3(0.0, 0.0, 0.0); // camera position

    vec3 lightAmbient  = gl_LightSource[0].ambient.xyz;
    vec3 lightDiffuse  = gl_LightSource[0].diffuse.xyz;
    vec3 lightSpecular = gl_LightSource[0].specular.xyz;

	vec3 materialAmbient;
	vec3 materialDiffuse;
	vec3 materialSpecular;
	if(colorMapping < 0.0){
		materialAmbient = gl_FrontMaterial.ambient.xyz;
		materialDiffuse = gl_FrontMaterial.diffuse.xyz;
		materialSpecular  = gl_FrontMaterial.specular.xyz;
	}
	else{
		materialAmbient = gl_FrontMaterial.ambient.xyz*texture2D(colorTex, texPos).xyz;
		materialDiffuse = gl_FrontMaterial.diffuse.xyz*texture2D(colorTex, texPos).xyz;
		materialSpecular  = gl_FrontMaterial.specular.xyz*texture2D(colorTex, texPos).xyz;
	}
    float shininess    = gl_FrontMaterial.shininess;

	/* CS 148 TODO: Implement the Phong reflectance model here */
	vec3 Lm = normalize(lightSourcePos-modelPos);
	vec3 Rm = normalize(reflect(-Lm,N));
	vec3 V = normalize(-modelPos);

	vec3 colorAmbient = materialAmbient*lightAmbient;
	vec3 colorDiffuse = clamp(max(dot(Lm,N),0.0)*materialDiffuse*lightDiffuse,0.0,1.0);
    vec3 colorSpecular = clamp(pow(max(dot(Rm,V),0.0),shininess)*materialSpecular*lightSpecular,0.0,1.0);

	vec3 color = colorAmbient + colorDiffuse + colorSpecular;
	color = clamp(color, 0.0, 1.0);

    gl_FragColor = vec4(color, 1.0);
}
