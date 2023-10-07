#!/usr/bin/env 

set -e

clang-format -i SoundEnginePlugin/**/*.h
clang-format -i SoundEnginePlugin/**/*.cpp
clang-format -i WwisePlugin/**/*.h
clang-format -i WwisePlugin/**/*.cpp