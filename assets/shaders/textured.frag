#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform vec4 color;
uniform bool useTexture;
uniform sampler2D tex;

void main() {
   if (useTexture) {
      fragColor = texture(tex, texCoord);
   } else {
      fragColor = color;
   }
}