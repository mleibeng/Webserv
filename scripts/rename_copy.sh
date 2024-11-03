#!/bin/zsh

# Put this into the directory where you want to rename the copy files to their normal name!
# !!! Make sure to delete the previous files before using it. (ofc)

for file in *.html; do
    if [[ -f $file ]]; then
        new_name=${file/ copy/}
        mv "$file" "$new_name"
    fi
done
