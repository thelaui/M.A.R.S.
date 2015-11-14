#!/bin/bash

# Exit as soon as any line in the bash script fails.
set -e

# Move up if we're not in the base directory.
if [ -d "../utils" ]; then
	pushd ..
fi

pushd po

# Print all commands.
#set -x

# NOCOM fix project id version and \n when everything works

# Now update the translations
for file in *.po; do
	echo "Updating translation for $file";
	file=${file##*/}
	msgmerge --no-wrap -F --lang=${file%.po} $file -o $file marsshooter.pot
done

# Now update the translation binaries
pushd ..

test -d data/locale || mkdir -p data/locale

for file in po/*.po; do
	echo "Updating translation binaries for $file";
	file=${file##*/}
	test -d "data/locale/"${file%.po} || mkdir -p "data/locale/"${file%.po}
	test -d "data/locale/"${file%.po}"/LC_MESSAGES" || mkdir -p "data/locale/"${file%.po}"/LC_MESSAGES"
	msgfmt -c -o "data/locale/"${file%.po}"/LC_MESSAGES/marsshooter.mo" "po/"${file%.mo}
done
