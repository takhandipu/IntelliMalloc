for (( d=1; d<=128; d*=2 ))
do
	for (( c=100; c<=2000; c+=100 ))
	do
		ocperf.py stat -o $d\_$c.txt -r 100 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./$d\_$c
	done
done
