#version 130

uniform sampler2D texture;
uniform float pixel_width;
uniform float pixel_height;
uniform float big_pixel;
uniform bool scanLines;
uniform bool blur;

out vec4 color;

void main()
{
	bool isDark = int(gl_TexCoord[0].y/big_pixel) % 2 == 0;
	vec4 ref = texture2D(texture, gl_TexCoord[0].xy);
	color = ref;
	if ((scanLines && isDark) || (blur && ref.r + ref.g + ref.b < .1)) {
		if (scanLines) {
			color = vec4(0, 0, 0, 1);
		}
		if (blur) {
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					vec4 pixel = texture2D(texture, vec2(gl_TexCoord[0].x + (pixel_width * j), gl_TexCoord[0].y + (pixel_height * i)));
					color += pixel;
					if (i == 0 && j == 0) {
						color += pixel * 18;
					}
				}
			}
			color /= 26;
			color.a = 1.0;
		}
	}
}
