#!/usr/bin/env sh

cd "$(dirname "$0")"
./vendor/Premake/bin/premake5 --cc=gcc gmake
cd -
