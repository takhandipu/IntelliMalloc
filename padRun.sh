for ((i=1; i<=128; i++));
do
g++ matrix_padded_parallel.cpp -DPADDING_DISTANCE=$i -lpthread -o matrix_padded_parallel$i
ocperf.py stat -o padded_parallel$i.txt -r 100 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./matrix_padded_parallel$i
done
