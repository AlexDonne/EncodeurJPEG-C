#!/usr/bin/bash

imageentree=$1
imagesortie=$2 
x="x"
vir=","
mkdir $imagesortie
for i in `seq 1 2` ; do
    for j in `seq 1 2` ; do
        for k in `seq 1 $i` ; do
            for l in `seq 1 $j` ; do
                for m in `seq 1 $i` ;do
                    for n in `seq 1 $j` ;do
                        if [ $i = 2 ] && [ $j = 2 ] && [ $k = 2 ] && [ $l = 2 ] && [ $m = 2 ] && [ $n = 2 ]; then 
                           break;
                       fi 
                        sample="$i$x$j$vir$k$x$l$vir$m$x$n"
                  echo $sample      
                        `./ppm2jpeg $imageentree --sample $sample --output-file $imagesortie/$sample$imagesortie.jpg`
                    done
                done
            done
        done
    done
done










