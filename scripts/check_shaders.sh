#!/bin/sh

# | sed -r 's/ERROR: ([0-9]+):([0-9]+):/\2:\1: error:/'
glslangValidator -l geometry_normalmap_pass.vert geometry_normalmap_pass.frag 
glslangValidator -l directionnal_light.vert directionnal_light.frag
glslangValidator -l full.vert full.frag
glslangValidator -l geometry_pass.vert geometry_pass.frag
glslangValidator -l geometry_normalmap_pass.vert geometry_normalmap_pass.frag
glslangValidator -l depth_only.vert depth_only.frag
glslangValidator -l fullscreen.vert fullscreen_depth.frag
glslangValidator -l fullscreen.vert fullscreen_normal.frag
glslangValidator -l fullscreen.vert fullscreen_color.frag
glslangValidator -l fullscreen.vert fullscreen_luminance.frag
glslangValidator -l fullscreen.vert fullscreen_luminance_mean.frag
glslangValidator -l fullscreen.vert gaussian_blur_h.frag
glslangValidator -l fullscreen.vert gaussian_blur_v.frag
