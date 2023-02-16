#!/bin/bash
# Compile redundans dependencies

echo `date` "Updating submodules..."
git submodule update --init --recursive 

echo `date` "Compiling dependencies..." 
cores=`grep -c ^processor /proc/cpuinfo | awk '{if($1>1){print $1-1} else {print 1}}'`
echo " I'll use $cores thread(s) for compiling"

echo `date` "Compiling bowtie2"
(cd /submodules/bowtie2/ && make)

echo `date` "Compiling bedtools2"
(cd /submodules/bedtools2/ && make)