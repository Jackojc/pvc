#!/usr/bin/env sh

die() {
	echo "$@"; exit 1
}

[ -z "$BUILD_DIR" ] && die "BUILD_DIR not set"
[ -z "$TEST_DIR" ]  && die "TEST_DIR not set"
[ -z "$TEST_FILE" ] && die "TEST_FILE not set"

test_files="$(find "$TEST_DIR" -type f -name '*.cpp')"

# total number of c files
total=$(echo "$test_files" | wc -l)
test_cases="$(cat "$TEST_FILE")"

i=1

# recursively find *.c files, compile them, run
# binary and then output its exit status.
echo "$test_files" |
while IFS="
" read -r file; do
	fname="$(basename "$file" .cpp)"

	# lookup basename of current test in tests
	expected=$(echo "$test_cases" | awk "/$fname/ { print \$2 }")

	[ -z "$expected" ] && die "$fname not found in test cases file"

	printf '%s\t' "[$i/$total] $fname"
	$BUILD_DIR/$fname

	[ $? -eq "$expected" ] \
		&& printf "[\033[42mok\033[0m]\n" \
		|| printf "[\033[41mfail\033[0m]\n"

	i=$(( i + 1 ))
done

