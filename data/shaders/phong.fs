
varying vec3 v_position;
varying vec3 v_world_position;
varying vec2 v_uv;
varying vec3 v_normal;

uniform vec4 u_background_color;
uniform vec3 u_camera_position;
uniform vec3 u_light_color;
uniform vec3 u_light_position;
uniform float u_fog_factor;

uniform sampler2D u_texture;

uniform vec3 u_Ks;
uniform vec3 u_Kd;
uniform vec3 u_Ka;

uniform vec2 u_maps;

uniform float u_tiling;

vec3 perturbNormal(vec3 N, vec3 V, vec2 uv, vec3 normal_texture) {
    // Convert normal map from [0,1] to [-1,1]
    vec3 normalTS = normalize(normal_texture * 2.0 - 1.0);

    // Compute Tangent and Bitangent vectors
    vec3 dPosition_dx = dFdx(v_world_position);
    vec3 dPosition_dy = dFdy(v_world_position);
    vec3 dUV_dx = dFdx(vec3(uv, 0.0));
    vec3 dUV_dy = dFdy(vec3(uv, 0.0));

    vec3 T = normalize(dUV_dy.y * dPosition_dx - dUV_dx.y * dPosition_dy);
    vec3 B = normalize(dUV_dx.x * dPosition_dy - dUV_dy.x * dPosition_dx);
    
    // Construct the Tangent-Bitangent-Normal matrix
    mat3 TBN = mat3(T, B, N);
    
    // Transform normal from tangent space to world space
    return normalize(TBN * normalTS);
}

vec4 applyLigth() {
	vec4 Kd = vec4(u_Kd, 1.0f);

	if (u_maps.x == 1) {
		Kd = texture2D(u_texture, v_uv);
	}

	vec3 L = normalize(u_light_position - v_world_position);
	vec3 V = normalize(u_camera_position - v_world_position);

	vec3 N = normalize(v_normal);

	if (u_maps.y == 1) {
		vec3 normal_texture = texture2D( u_normals_texture, v_uv );
		N = perturbNormal(normalize(v_normal), -V, v_uv, normal_texture);
	}

	// ambient component
	vec3 ambient = u_Ka * Kd * u_light_color * 0.4;
	
	float steps = 5.0f;

	// diffuse component
	float NdotL = max(0.0, dot(L,N));
	NdotL = floor(NdotL * steps) / steps;
	vec3 diffuse = Kd * u_light_color * NdotL;

	// specular component
	vec3 R = reflect(-L,N);
	float RdotV = pow(max(0.0, dot(R,V)), 20);
	RdotV = floor(RdotV * steps) / steps;
	vec3 specular = u_Ks * u_light_color * RdotV;

	// total light
	vec3 total_light = ambient + diffuse + specular;
	return vec4(total_light, Kd.a);
}

void main() {
	vec4 final_color = applyLight();

	float dist = length(u_camera_position - v_world_position);

	float fogMaxDist = 4.0;
	float fogValue = clamp(1.0 - (fogMaxDist / dist), 0.0, 1.0);
	fogValue *= u_fog_factor;
	
	final_color.rgb = mix(final_color.rgb, vec3(0.7,0.6,0.7), fogValue);

	gl_FragColor = final_color;
}
