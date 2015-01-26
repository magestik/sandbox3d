
struct VS_OUTPUT
{
	vec2 texCoord;
};

uniform sampler2D texSampler;
uniform vec2 texelSize;

in VS_OUTPUT vsOut;

out vec2 outLuminance;

void main(void)
{
	vec2 p1 = texture(texSampler, vsOut.texCoord).rg;
	vec2 p2 = textureOffset(texSampler, vsOut.texCoord, ivec2(0, 1)).rg;
	vec2 p3 = textureOffset(texSampler, vsOut.texCoord, ivec2(1, 0)).rg;
	vec2 p4 = textureOffset(texSampler, vsOut.texCoord, ivec2(1, 1)).rg;

	outLuminance.r = (p1.r + p2.r + p3.r + p4.r) * 0.25;
	outLuminance.g = max(max(p1.g, p2.g), max(p3.g, p4.g));
}
