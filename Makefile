DEST=data/shaders/

all: copy check
	echo $(DEST)
copy:
	mkdir -p $(DEST)
	cp src/Rendering/Shader/shaders/*.frag data/shaders/
	cp src/Rendering/Shader/shaders/*.vert data/shaders/

check:
	# | sed -r 's/ERROR: ([0-9]+):([0-9]+):/\2:\1: error:/'
	glslangValidator -l $(DEST)/geometry_normalmap_pass.vert $(DEST)/geometry_normalmap_pass.frag 
	glslangValidator -l $(DEST)/directionnal_light.vert $(DEST)/directionnal_light.frag
	glslangValidator -l $(DEST)/full.vert $(DEST)/full.frag
	glslangValidator -l $(DEST)/geometry_pass.vert $(DEST)/geometry_pass.frag
	glslangValidator -l $(DEST)/geometry_normalmap_pass.vert $(DEST)/geometry_normalmap_pass.frag
	glslangValidator -l $(DEST)/depth_only.vert $(DEST)/depth_only.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/fullscreen_depth.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/fullscreen_normal.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/fullscreen_color.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/fullscreen_luminance.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/fullscreen_luminance_mean.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/gaussian_blur_h.frag
	glslangValidator -l $(DEST)/fullscreen.vert $(DEST)/gaussian_blur_v.frag
