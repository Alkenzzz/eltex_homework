#!/bin/bash

for i in {1..20}; do
	mkdir -p "$i"
	touch "$i/$i.txt"
done

for i in {1..20}; do
	ln -s "$i/$i.txt" "$i.txt"
done
for i in {1..20}; do
	echo "$i/$i.txt" > "$i/$i.txt" 
done

for i in {1..20}; do
	if ! grep -q 4 "$i/$i.txt"; then
		rm "$i/$i.txt"
		rm "$i.txt"
	fi
done
echo "Новое содержимое файла 14/14.txt" > 14/14.txt

