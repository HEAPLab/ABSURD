#!/bin/bash


CHRONO_APP_BIN=/mnt/data/chronovise/build/apps/chronoapp/chronovise-app

for platform in `ls exec_times/`; do
	mkdir -p pwcet/$platform
	for class in `ls exec_times/$platform`; do
		mkdir -p pwcet/$platform/$class
		for F in `ls exec_times/$platform/$class/*.dat`; do
			echo "Running: " ${CHRONO_APP_BIN} ${F}
			RES=$(${CHRONO_APP_BIN} --use-double --skip 10 ${F})
			FILENAME=${F##*/}
			#echo "$RES" | grep -A3 "KPSS test result"		> "pwcet/$platform/$class/tests_${FILENAME%.*}.txt"
			echo "$RES" | grep -A7 "Estimated distribution" > "pwcet/$platform/$class/pwcet_${FILENAME%.*}.txt"
		done
	done
done

