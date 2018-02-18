#!/bin/sh

mkdir -p data/shaders/

FRAG=src/Rendering/Shader/shaders/*.frag
VERT=src/Rendering/Shader/shaders/*.vert
GEOM=src/Rendering/Shader/shaders/*.geom

DEST=data/shaders/

for f in $FRAG $VERT $GEOM
do
	if [ -f $f ]; then
		# TODO : use the shaderc instead
		name=$(basename "$f")
		/mnt/workspace/ShadersCompilers/shaderc/build/glslc/glslc -std=330 -E $f -I src/Rendering/Shader/shaders/ | sed 's/^#extension GL_GOOGLE_include_directive : enable$/ /' | sed 's/^#line [0-9]* .*$/ /' > $DEST/$name
	fi
done

