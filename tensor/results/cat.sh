for (( d=100; d<=2000; d+=100 ))
do
	python mine.py $d > $d.csv
done
