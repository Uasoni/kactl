#!/usr/bin/env bash
set -u

directory=${1:-.}
script_directory="$directory/doc/scripts"
compiler_flags=(-Wall -Wextra -Wfatal-errors -Wconversion -std=c++20)

g++ "${compiler_flags[@]}" -x c++-header "$directory/content/contest/template.cpp"
trap 'rm -f "$directory/content/contest/template.cpp.gch"' EXIT

mapfile -t headers < <(find "$directory/content" -name '*.h' | sort)
tests=()
skipped=()
for header in "${headers[@]}"; do
	if grep -qFf "$script_directory/skip_headers" <<< "$header"; then
		skipped+=("$header")
	else
		tests+=("$header")
	fi
done

printf 'skipped:\n'
printf ' %s\n' "${skipped[@]}"

pass=0
failed_headers=()
for test in "${tests[@]}"; do
	printf '%s: ' "$(basename "$test")"
	if "$script_directory/test-compiles.sh" "$test" "$directory"; then
		printf 'passed\n'
		((pass += 1))
	else
		printf 'failed\n'
		failed_headers+=("$test")
	fi
done

total=${#tests[@]}
printf '%d/%d tests passed\n' "$pass" "$total"
if ((total == 0)); then
	printf 'No tests found; check skip_headers for blank lines.\n'
	exit 1
fi
if ((${#failed_headers[@]})); then
	printf 'These tests failed:\n'
	printf ' %s\n' "${failed_headers[@]}"
	exit 1
fi
printf 'No tests failed\n'
