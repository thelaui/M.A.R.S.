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

test -d build/locale || mkdir -p build/locale
test -e locale || ln -s build/locale

for file in po/*.po; do
	echo "Updating translation binaries for $file";
	file=${file##*/}
	test -d "locale/"${file%.po} || mkdir -p "locale/"${file%.po}
	test -d "locale/"${file%.po}"/LC_MESSAGES" || mkdir -p "locale/"${file%.po}"/LC_MESSAGES"
	msgfmt -c -o "locale/"${file%.po}"/LC_MESSAGES/marsshooter.mo" "po/"${file%.mo}
done
