#!/bin/bash

# Exit as soon as any line in the bash script fails.
set -e

# Move up if we're not in the base directory.
if [ -d "../utils" ]; then
	pushd ..
fi

# Print all commands.
set -x

# Now build the catalog
find src -iname "*.cpp" | xargs xgettext --from-code utf-8 -o - -p po -k_ --c++ -F -c"* TRANSLATORS" --package-name="M.A.R.S." --no-wrap -o marsshooter.pot
