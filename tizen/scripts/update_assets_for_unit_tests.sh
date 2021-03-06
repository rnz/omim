#!/bin/bash
set -x -u

SRC=../../../data
DST=$1

# Remove old links
#rm -rf $DST
#mkdir $DST

files=(copyright.html resources-ldpi resources-mdpi resources-hdpi resources-xhdpi resources-xxhdpi categories.txt classificator.txt
       types.txt fonts_blacklist.txt fonts_whitelist.txt languages.txt unicode_blocks.txt \
       drules_proto.bin packed_polygons.bin countries.txt World.mwm WorldCoasts.mwm 00_roboto_regular.ttf 01_dejavusans.ttf 02_droidsans-fallback.ttf
       03_jomolhari-id-a3d.ttf 04_padauk.ttf 05_khmeros.ttf 06_code2000.ttf
       minsk-pass.mwm)

for item in ${files[*]}
do
  ln -s $SRC/$item $DST/$item
done
