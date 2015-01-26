#!/bin/sh

mkdir -p data/shaders/


FRAG=src/Rendering/Shader/shaders/*.frag
VERT=src/Rendering/Shader/shaders/*.vert

DEST=data/shaders/

for f in $FRAG $VERT
do
	name=$(basename "$f")
	echo "#version 330" > $DEST/$name
	cpp -P -x c++ -nostdinc -nostdinc++ $f >> $DEST/$name
done

cd build
ctest check_shaders -VV

