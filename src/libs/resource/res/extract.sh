#! /usr/bin/env bash
set -ex

# normal, hover, pressed, disable
function split_btn
{
    src=$1
    n=$2
    read w h d <<< $(file $src.bmp | cut -d, -f3| sed -e 's/x/\n/g')
    echo "width, height, depth: $w, $h, $d"
    ((w /= n))
    echo $w;
    for i in $(eval "echo $(echo {0..$((n-1))})")
    do
        offset=$((i * w))
        echo "convert -extract ${w}x${h}+${offset}+0 $src.bmp $src-sth.bmp"
    done
        
}

split_btn adsorb 8
