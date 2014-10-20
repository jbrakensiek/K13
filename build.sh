#!/bin/bash
rm *.gch &&
gcc -c -O2 naugraph.c nautil.c naututil.c naututil.h nauty.c nauty.h rng.c rng.h &&
for (( i = 4; i <= 10; i++));
do
	echo "AT $i"
	sed "s/\#define N .*/\#define N $i/g" -i main.cpp 
	g++ -O2 main.cpp dgraph.h *.o -w &&
	echo "" | ./a.out &&
	for f in $(ls | grep "\#.*K$((i))to$((i+1)).txt")
	do
		mv -v "$f" $(echo $f | sed "s/$((i))to$((i+1))/$((i+1))/")
	done
done
