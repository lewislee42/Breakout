#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

// note szie values must be passed from code
const float renderWidth = 400;
const float renderHeight = 400;

void main()
{
  float caStrength = 0.003;
  vec2 caOffset = fragTexCoord - 0.4;

  vec2 caUVR = fragTexCoord + caOffset * caStrength * 1.2;
  vec2 caUVG = fragTexCoord + caOffset * caStrength;
  vec2 caUVB = fragTexCoord + caOffset * caStrength * 1.4;

  vec3 color;
  color.x = texture(texture0, caUVR).x;
  color.y = texture(texture0, caUVG).y;
  color.z = texture(texture0, caUVB).z;

  vec2 size = vec2(renderWidth, renderHeight);
  float samples = 5;
  float quality = 0.8;

  vec4 sum = vec4(0.0);
  vec2 sizeFactor = vec2(1) / size * quality;
  // vec4 source = texture(texture0, fragTexCoord);
  vec4 source = vec4(color, 1.0);
  const int range = 2;
  for (int x = -range; x <= range; x++) {
    for (int y = -range; y <= range; y++) {
      sum += texture(texture0, fragTexCoord + vec2(x, y) * sizeFactor);
    }
  }

  color = vec4(((sum / (samples * samples)) + source) * colDiffuse).rgb;

  float scanline = sin(fragTexCoord.y * 800.0) * 0.15;
  color -= scanline;
  finalColor = vec4(color, 1.0);
}

