#!/usr/bin/env sh

cd "$(dirname "$0")/.."
./vendor/Premake/bin/premake5-linux --cc=gcc gmake
cd -
