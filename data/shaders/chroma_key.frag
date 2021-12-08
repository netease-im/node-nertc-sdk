#version 330 core
uniform sampler2D image;

const mat4 yuv_mat = mat4(   0.182586,  0.614231,  0.062007, 0.062745,
							-0.100644, -0.338572,  0.439216, 0.501961,
							 0.439216, -0.398942, -0.040274, 0.501961,
							 0.000000,  0.000000,  0.000000, 1.000000);

uniform vec4 color = {1.0,1.0,1.0,1.0};
uniform float contrast = 1.0;
uniform float brightness = 0.0;
uniform float gamma = 1.0;

uniform vec2 chroma_key = {0.1634, 0.103};
uniform vec2 pixel_size = {0.01, 0.01};
uniform float similarity = 0.4;
uniform float smoothness = 0.08;
uniform float spill = 0.1;

varying vec2 cur_pt;

vec4 CalcColor(vec4 rgba)
{
	return vec4(pow(rgba.rgb, vec3(gamma, gamma, gamma)) * contrast + brightness, rgba.a);
}

float GetChromaDist(vec3 rgb)
{
	vec4 yuvx = vec4(rgb.rgb, 1.0) * yuv_mat;
	return distance(chroma_key, yuvx.yz);
}
vec4 SampleTexture(vec2 texCoord)
{
	return texture2D(image, texCoord);
}
float GetBoxFilteredChromaDist(vec3 rgb, vec2 texCoord)
{
	vec2 h_pixel_size = pixel_size / 2.0;
	vec2 point_0 = vec2(pixel_size.x, h_pixel_size.y);
	vec2 point_1 = vec2(h_pixel_size.x, -pixel_size.y);
	float distVal = GetChromaDist(SampleTexture(texCoord-point_0).rgb);
	distVal += GetChromaDist(SampleTexture(texCoord+point_0).rgb);
	distVal += GetChromaDist(SampleTexture(texCoord-point_1).rgb);
	distVal += GetChromaDist(SampleTexture(texCoord+point_1).rgb);
	distVal *= 2.0;
	distVal += GetChromaDist(rgb);
	return distVal / 9.0;
}
float saturate(float v)
{
	return clamp(v, 0, 1);
}
vec3 saturate3v(vec3 v)
{
	return vec3(saturate(v.r), saturate(v.b), saturate(v.b));
}
vec4 ProcessChromaKey(vec2 texCoord)
{
	vec4 rgba = SampleTexture(texCoord);
	float chromaDist = GetBoxFilteredChromaDist(rgba.rgb, texCoord);
	float baseMask = chromaDist - similarity;
	float fullMask = pow(saturate(baseMask / smoothness), 1.5);
	float spillVal = pow(saturate(baseMask / spill), 1.5);

	rgba.rgba *= color;
	rgba.a *= fullMask;

	float desat = (rgba.r * 0.2126 + rgba.g * 0.7152 + rgba.b * 0.0722);
	rgba.rgb = saturate3v(vec3(desat, desat, desat)) * (1.0 - spillVal) + rgba.rgb * spillVal;

	return CalcColor(rgba);
}

void main()
{
	gl_FragColor = ProcessChromaKey(cur_pt);
}