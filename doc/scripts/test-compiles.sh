#!/usr/bin/env bash
set -u

header=$1
directory=${2:-.}
workspace=$(realpath "$directory")
temporary_directory=$(mktemp -d)
trap 'rm -rf "$temporary_directory"' EXIT

printf '#include "%s/content/contest/template.cpp"\n#include "%s/%s"\n' \
	"$workspace" "$workspace" "${header#./}" > "$temporary_directory/test.cpp"

g++ -Wall -Wextra -Wfatal-errors -Wconversion -std=c++20 \
	"$temporary_directory/test.cpp" -o "$temporary_directory/test"
