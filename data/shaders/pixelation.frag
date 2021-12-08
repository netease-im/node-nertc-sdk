varying vec4 color;
varying vec2 texCoord;

uniform sampler2D tex; // 0
uniform float vx_offset;    //offset of the vertical red line
uniform float rt_w; // render target width
uniform float rt_h; // render target height
uniform float pixel_w; // 15.0, width of a low resolution pixel
uniform float pixel_h; // 10.0, height of a low resolution pixel

void main() 
{ 
  vec4 tc;
  if (texCoord.x < (vx_offset-0.005))
  {
    float dx = pixel_w*(1./rt_w);
    float dy = pixel_h*(1./rt_h);
    vec2 coord = vec2(dx*floor(texCoord.x/dx),
                      dy*floor(texCoord.y/dy));
    tc = texture2D(tex, coord);
  }
  else if (texCoord.x>=(vx_offset+0.005))
  {
    tc = texture2D(tex, texCoord);
  }
  gl_FragColor = tc;
}