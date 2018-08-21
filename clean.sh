#!/bin/sh -
for i in p*
do
	make -C "$i" clean
done
