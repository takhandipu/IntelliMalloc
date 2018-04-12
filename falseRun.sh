for i in {1,2,4,8,16};
do
ocperf.py stat -o false$i.txt -r 100 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./false$i
done
