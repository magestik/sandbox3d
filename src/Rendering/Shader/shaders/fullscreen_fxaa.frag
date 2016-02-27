#version 330 core

#include "../Interface.h"

#include "fullscreen.h"

#define FXAA_PC 1
#define FXAA_GLSL_130 1
#define FXAA_QUALITY__PRESET 12
#define FXAA_GREEN_AS_LUMA 0
#define FXAA_FAST_PIXEL_OFFSET 0
#define FXAA_GATHER4_ALPHA 0

#include "Fxaa3_11.h"

uniform sampler2D texSampler;

in VS_OUTPUT vsOut;

out vec3 outColor;

uniform FxaaFloat2 fxaaQualityRcpFrame;
uniform FxaaFloat4 fxaaConsoleRcpFrameOpt;
uniform FxaaFloat4 fxaaConsoleRcpFrameOpt2 ;
uniform FxaaFloat4 fxaaConsole360RcpFrameOpt2;

uniform FxaaFloat fxaaQualitySubpix = 0.75;
uniform FxaaFloat fxaaQualityEdgeThreshold = 0.166;
uniform FxaaFloat fxaaQualityEdgeThresholdMin = 0.0833;

uniform FxaaFloat fxaaConsoleEdgeSharpness = 8.0;
uniform FxaaFloat fxaaConsoleEdgeThreshold = 0.125;
uniform FxaaFloat fxaaConsoleEdgeThresholdMin = 0.05;
uniform FxaaFloat4 fxaaConsole360ConstDir = vec4(1.0, -1.0, 0.25, -0.25);

void main(void)
{
	vec4 fxaaConsolePosPos = vec4(0.0, 0.0, 0.0, 0.0);
	outColor = FxaaPixelShader(vsOut.texCoord, fxaaConsolePosPos, texSampler, texSampler, texSampler, fxaaQualityRcpFrame, fxaaConsoleRcpFrameOpt, fxaaConsoleRcpFrameOpt2, fxaaConsole360RcpFrameOpt2, fxaaQualitySubpix, fxaaQualityEdgeThreshold, fxaaQualityEdgeThresholdMin, fxaaConsoleEdgeSharpness, fxaaConsoleEdgeThreshold, fxaaConsoleEdgeThresholdMin, fxaaConsole360ConstDir).rgb;
}


