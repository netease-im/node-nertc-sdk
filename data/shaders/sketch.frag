#ifdef GL_ES
precision mediump float;
#endif

varying vec2 texCoord;

uniform sampler2D tex;
uniform vec2 iResolution;     //input 

const vec4 luminance_vector = vec4(0.3, 0.59, 0.11, 0.0);

void main() {
	vec2 uv = vec2(1.0) - (texCoord / iResolution);
	vec2 n = 1.0/iResolution;
	vec4 CC = texture2D(tex, uv);
	vec4 RD = texture2D(tex, uv + vec2( n.x, -n.y));
	vec4 RC = texture2D(tex, uv + vec2( n.x,  0.0));
	vec4 RU = texture2D(tex, uv + n);
	vec4 LD = texture2D(tex, uv - n);
	vec4 LC = texture2D(tex, uv - vec2( n.x,  0.0));
	vec4 LU = texture2D(tex, uv - vec2( n.x, -n.y));
	vec4 CD = texture2D(tex, uv - vec2( 0.0,  n.y));
	vec4 CU = texture2D(tex, uv + vec2( 0.0,  n.y));

	gl_FragColor = vec4(2.0*abs(length(
		vec2(
			-abs(dot(luminance_vector, RD - LD))
			+4.0*abs(dot(luminance_vector, RC - LC))
			-abs(dot(luminance_vector, RU - LU)),
			-abs(dot(luminance_vector, LD - LU))
			+4.0*abs(dot(luminance_vector, CD - CU))
			-abs(dot(luminance_vector, RD - RU))
		)
	)-0.5));
}