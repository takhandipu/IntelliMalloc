for (( c=100; c<=2000; c+=100 ))
do
  ocperf.py stat -o $c.txt -r 100 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./$c
done
