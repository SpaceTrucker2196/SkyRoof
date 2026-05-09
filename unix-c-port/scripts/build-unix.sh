#!/usr/bin/env sh
set -eu

repoRoot="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
buildDir="${repoRoot}/build"

cmake -S "${repoRoot}" -B "${buildDir}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${buildDir}" --parallel
ctest --test-dir "${buildDir}" --output-on-failure
