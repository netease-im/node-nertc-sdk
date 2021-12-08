varying vec2 texCoord;

uniform sampler2D tex;
uniform vec2 iResolution;     //input 

// Table of pigments 
// from Computer-Generated Watercolor. Cassidy et al.
// K is absortion. S is scattering
vec3 K_QuinacridoneRose = vec3(0.22, 1.47, 0.57);
vec3 S_QuinacridoneRose = vec3(0.05, 0.003, 0.03);
vec3 K_FrenchUltramarine = vec3(0.86, 0.86, 0.06);
vec3 S_FrenchUltramarine = vec3(0.005, 0.005, 0.09);
vec3 K_CeruleanBlue = vec3(1.52, 0.32, 0.25);
vec3 S_CeruleanBlue = vec3(0.06, 0.26, 0.40);
vec3 K_HookersGreen = vec3(1.62, 0.61, 1.64);
vec3 S_HookersGreen = vec3(0.01, 0.012, 0.003);
vec3 K_HansaYellow = vec3(0.06, 0.21, 1.78);
vec3 S_HansaYellow = vec3(0.50, 0.88, 0.009);
 
// Math functions not available in webgl
vec3 cosh(vec3 val) { vec3 e = exp(val); return (e + vec3(1.0) / e) / vec3(2.0); }
vec3 tanh(vec3 val) { vec3 e = exp(val); return (e - vec3(1.0) / e) / (e + vec3(1.0) / e); }
vec3 sinh(vec3 val) { vec3 e = exp(val); return (e - vec3(1.0) / e) / vec3(2.0); }
 
// Kubelka-Munk reflectance and transmitance model
void KM(vec3 k, vec3 s, float x, out vec3 refl, out vec3 trans)
{
   // 5.1 Specifying the optical properties of pigments
    vec3 a = (k+s)/s; // K = S(a - 1)
    vec3 b = sqrt(a*a - vec3(1.0));
    vec3 bsx = b*s*vec3(x);
    vec3 sinh_bsx = sinh(bsx);
    vec3 denom = b*cosh(bsx)+a*sinh_bsx;
    refl = sinh_bsx/denom;
    trans = b/denom;
}
 
// The watercolours tends to dry first in the center
// and accumulate more pigment in the corners
float BrushEffect(float dist, float h_avg, float h_var)
{
    float h = max(0.0,1.0-10.0*abs(dist));
    h = pow(h, 4);
    return (h_avg + h_var*h) * smoothstep(-0.01, 0.002, dist);
}
 
// Kubelka-Munk model for layering
void CompositeLayers(vec3 r0, vec3 t0, vec3 r1, vec3 t1, out vec3 r, out vec3 t)
{
    // 5.2 Optical compositing of layers
    r = r0 + t0*t0*r1 / (vec3(1.0)-r0*r1);
    t = t0*t1 / (vec3(1.0)-r0*r1);
}
 
// Simple 2d noise fbm with 3 octaves
float Noise2d(vec2 p)
{
    float t = texture2D(tex, p).x;
    t += 0.5 * texture2D(tex, p * 2.0).x;
    t += 0.25 * texture2D(tex, p * 4.0).x;
    return t / 1.75;
}
 
float DistanceCircle(vec2 pos, vec2 center, float radius) 
{
   vec2 offset = pos - center;
   return 1.2 * radius - length(offset);
   
    //return 1.0 - distance(pos, center) / radius
}
 
float DistanceSegment(vec2 pos, vec2 pt1, vec2 pt2, float delta)
{
   float dist = 0;
 
   vec2 a = pos - pt1;
   vec2 b = pt2 - pt1;
   // "clamp" restrain the length of the vector "c"
   // |a|*cos(theta) / |b|
   vec2 c = b * clamp(dot(a, b)/dot(b, b), 0.0, 1.0);
   vec2 e = c - a;
   // delta control the width of the line
   dist = 0.5*delta - length(e);
   return dist;
}
 
void main(void)
{
   vec2 uv = texCoord / iResolution.xy;
   vec3 R0,T0,R1,T1;
   
  // Background
  float background = 0.1 + 0.2 * Noise2d(uv * vec2(1.0));
  KM(K_HansaYellow, S_HansaYellow, background, R0, T0);
 
  // Edge roughness: 0.04
  vec2 pos = uv * vec2(1.0, iResolution.y / iResolution.x) + vec2(0.04 * Noise2d(uv * vec2(0.1)));
  float dist = DistanceCircle(pos, vec2(0.5, 0.5), 0.15);
  // Average thickness: 0.2, edge varing thickness: 0.2
  float circle = BrushEffect(dist, 0.2, 0.2);
  // Granulation: 0.85
  circle *= 0.15 + 0.85 * Noise2d(uv * vec2(0.2));
  KM(K_QuinacridoneRose, S_QuinacridoneRose, circle, R1, T1);
  // 将上次计算的R0 T0与新计算的R1 T1 进行合成，并生成新的R0 T0
  CompositeLayers(R0, T0, R1, T1, R0, T0);
 
  // Edge roughness: 0.03
  pos = uv * vec2(1.0, iResolution.y / iResolution.x) + vec2(0.03 * Noise2d(uv * vec2(0.1)));
  dist = DistanceCircle(pos, vec2(0.4, 0.3), 0.15);
  // Average thickness: 0.3, edge varing thickness: 0.1
  circle = BrushEffect(dist, 0.3, 0.1);
  // Granulation: 0.65
  circle *= 0.35 + 0.65 * Noise2d(uv * vec2(0.2));
  KM(K_CeruleanBlue, S_CeruleanBlue, circle, R1, T1);
  CompositeLayers(R0, T0, R1, T1, R0, T0);
 
  // Edge roughness: 0.02
  pos = uv * vec2(1.0, iResolution.y / iResolution.x) + vec2(0.02 * Noise2d(uv * vec2(0.1)));
  dist = DistanceCircle(pos, vec2(0.6, 0.3), 0.15);
  // Average thickness: 0.3, edge varing thickness: 0.2
  circle = BrushEffect(dist, 0.3, 0.2);
  // Granulation: 0.45
  circle *= 0.55 + 0.45 * Noise2d(uv * vec2(0.2));
  KM(K_FrenchUltramarine, S_FrenchUltramarine, circle, R1, T1);
  CompositeLayers(R0, T0, R1, T1, R0, T0);
 
  // Opaque paints, e.g. Indian Red
  pos = uv * vec2(1.0, iResolution.y / iResolution.x) + vec2(0.02 * Noise2d(uv * vec2(0.3)));
  dist = DistanceSegment(pos, vec2(0.2, 0.1), vec2(0.4, 0.25), 0.03);
  float line = BrushEffect(dist, 0.2, 0.1);
  KM(K_HansaYellow, S_HansaYellow, line, R1, T1);
  CompositeLayers(R0, T0, R1, T1, R0, T0);
 
  // Transparent paints, e.g. Quinacridone Rose
  pos = uv * vec2(1.0, iResolution.y / iResolution.x) + vec2(0.02 * Noise2d(uv * vec2(0.2)));
  dist = DistanceSegment(pos, vec2(0.2, 0.5), vec2(0.4, 0.55), 0.03);;
  line = BrushEffect(dist, 0.2, 0.1);
  KM(K_QuinacridoneRose, S_QuinacridoneRose, line, R1, T1);
  CompositeLayers(R0, T0, R1, T1, R0, T0);
 
  // Interference paints, e.g. Interference Lilac
  pos = uv * vec2(1.0, iResolution.y / iResolution.x) + vec2(0.02 * Noise2d(uv * vec2(0.1)));
  dist = DistanceSegment(pos, vec2(0.6, 0.55), vec2(0.8, 0.4), 0.03);
  line = BrushEffect(dist, 0.2, 0.1);
  KM(K_HookersGreen, S_HookersGreen, line, R1, T1);
  CompositeLayers(R0, T0, R1, T1, R0, T0);
        
  gl_FragColor = vec4(R0+T0, 1.0);
}