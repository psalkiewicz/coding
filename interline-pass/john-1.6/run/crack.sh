#!/bin/bash

john -incremental:all -stdout | while read i; do

	echo -n "$i: "
	./code $i

done

