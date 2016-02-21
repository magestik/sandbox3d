#!/bin/sh

mkdir -p data/shaders/


FRAG=src/Rendering/Shader/shaders/*.frag
VERT=src/Rendering/Shader/shaders/*.vert
GEOM=src/Rendering/Shader/shaders/*.geom

DEST=data/shaders/

for f in $FRAG $VERT $GEOM
do
	# TODO : use the shaderc instead
	name=$(basename "$f")
	cpp -P -x c++ -nostdinc -nostdinc++ $f | sed 's/^#pragma GLSL(\(.*\)) \(.*\)$/#\1 \2/' > $DEST/$name
done

#cd build
#ctest check_shaders -VV

