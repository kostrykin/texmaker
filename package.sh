#!/bin/bash

mkdir dist
mkdir -p dist/usr/bin dist/usr/share/applications dist/usr/share/pixmaps
cp texmaker dist/usr/bin/
cp utilities/texmaker.desktop dist/usr/share/applications/
cp utilities/texmaker.png dist/usr/share/pixmaps/

fpm -s dir -t deb -n texmaker-kostrykin -v "$1" \
        --url "https://github.com/kostrykin/texmaker" \
        -d libqt4 -d zlib1g -d libpoppler-qt4 \
        --conflicts texmaker \
        -C dist --prefix / usr \

rm -Rf dist

