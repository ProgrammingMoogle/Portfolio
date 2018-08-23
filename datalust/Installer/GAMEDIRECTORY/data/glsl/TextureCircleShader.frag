#version 330
uniform vec2 tex_offset;
uniform sampler2D samp;
uniform vec4 color;
uniform float alpha;
uniform int blendmode;
in vec2 TexCoord;
out vec4 FragColor;

in vec2 CUV;

vec4 Mix(vec4 dst, vec4 src) {
	return vec4((1 - src.a) * dst.rgb + src.a * src.rgb, dst.a);
}

float MultiplyChannel(float dst, float src) {
	return dst * src;
}

vec4 Multiply(vec4 dst, vec4 src) {
	return Mix( dst,
		vec4(
			MultiplyChannel(dst.r, src.r),
			MultiplyChannel(dst.g, src.g),
			MultiplyChannel(dst.b, src.b),
			src.a
		)
	);
}

float ScreenChannel(float dst, float src) {
	return 1 - (1 - dst) * (1 - src);
}

vec4 Screen(vec4 dst, vec4 src) {
	return Mix( dst,
		vec4(
			ScreenChannel(dst.r, src.r),
			ScreenChannel(dst.g, src.g),
			ScreenChannel(dst.b, src.b),
			src.a
		)
	);
}

float OverlayChannel(float dst, float src) {
	if(dst < 0.5) {
		return 2 * dst * src;
	} else {
		return 1 - 2 * (1 - dst) * (1 - src);
	}
}

vec4 Overlay(vec4 dst, vec4 src) {
	return Mix( dst,
		vec4(
			OverlayChannel(dst.r, src.r),
			OverlayChannel(dst.g, src.g),
			OverlayChannel(dst.b, src.b),
			src.a
		)
	);
}

void main() {
  if( (CUV.x * CUV.x) + (CUV.y * CUV.y) > 1 ) {
	discard;
  }

  vec4 Color = texture(samp, TexCoord + tex_offset);

  switch(blendmode) {
	case 0: // none
	break;
	case 1: // Mix
		Color = Mix(Color, color);
	break;
	case 2: // Multiply
		Color = Multiply(Color, color);
	break;
	case 3: // Screen
		Color = Screen(Color, color);
	break;
	case 4: // Overlay
		Color = Overlay(Color, color);
	break;
  }

  FragColor = vec4(Color.rgb, alpha * Color.a );
}