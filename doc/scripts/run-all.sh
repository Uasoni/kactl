#!/usr/bin/env bash

directory=${1:-.}
compiler_flags=(-Wall -Wextra -Wfatal-errors -Wconversion -std=c++20 -O2)
temporary_directory=$(mktemp -d)

# use a precompiled header for the template to improve perf
g++ "${compiler_flags[@]}" -x c++-header \
	"$directory/stress-tests/utilities/template.h"
cleanup() {
	rm -f "$directory/stress-tests/utilities/template.h.gch"
	rm -rf "$temporary_directory"
}
trap cleanup EXIT

mapfile -t tests < <(find "$directory/stress-tests" -name '*.cpp' | sort)
pass=0
fail=0
failed_tests=()
ulimit -s 524288 # For 2-sat test
for test in "${tests[@]}"; do
	echo "$(basename "$test"):"
	start=$(date +%s.%N)
	executable="$temporary_directory/$(basename "$test" .cpp)"
	if g++ "${compiler_flags[@]}" "$test" -o "$executable" \
			&& "$executable"; then
		((pass += 1))
	else
		status=$?
		echo "Failed with $status"
		((fail += 1))
		failed_tests+=("$test")
	fi
	end=$(date +%s.%N)
	runtime=$(awk -v start="$start" -v end="$end" 'BEGIN {print end - start}')
	echo "Took $runtime seconds"
	echo
done
echo "$pass/$((pass + fail)) tests passed"
if (($fail == 0)); then
	echo "No tests failed"
	exit 0
else
	printf 'These tests failed:\n'
	printf ' %s\n' "${failed_tests[@]}"
	exit 1
fi
