#!/bin/bash

cat john-1.6/run/dupa | while read i; do echo -n "$i: "; ./code $i; done | grep 4030

