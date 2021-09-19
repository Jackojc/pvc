#!/usr/bin/env python3


# https://en.wikipedia.org/wiki/Unicode_character_property
# https://www.unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
# https://www.unicode.org/reports/tr44/#UnicodeData.txt
# https://www.unicode.org/reports/tr44/#General_Category_Values


import sys

def generate_or(values):
	values.sort()
	out = []

	i = 0

	while i < len(values) - 1:
		# Collapse consecutive ranges
		if values[i] == values[i + 1] - 1:
			range = []

			while i < len(values) - 1 and values[i] == values[i + 1] - 1:
				range.append(values[i])
				i += 1

			range.append(values[i])
			i += 1

			mn = min(range)
			mx = max(range)

			if mn == 0:
				out.append(f"(c <= {mx})")

			else:
				out.append(f"((c >= {mn}) and (c <= {mx}))")

		# Lone value
		else:
			out.append(f"(c == {values[i]})")
			i += 1

	# We skip the loop if size is 1 so we check here instead
	if len(values) == 1:
		out.append(f"(c == {values[i]})")

	return " or ".join(out)


def generate_function(fn, values):
	return f"constexpr bool {fn}(char_t c) {{ return ({generate_or(values)}); }}"


with open(sys.argv[1]) as f:
	codepoints = f.readlines()

lookup = {}

for c in codepoints:
	code, \
	name, \
	general_category, \
	canonical_combining_class, \
	bidi_class, \
	decomposition, \
	numeric_type, numeric_digit, numeric_numeric, \
	bidi_mirrored, \
	uc1name, \
	iso_comment, \
	simple_uppercase_mapping, \
	simple_lowercase_mapping, \
	simple_titlecase_mapping, \
		= c.split(";")

	key = general_category.lower()

	lookup.setdefault(key, []);
	lookup[key].append(int(code, 16))

for key, value in lookup.items():
	print(generate_function("is_" + key, value))
