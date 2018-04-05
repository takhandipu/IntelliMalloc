g++ matrix_padded_parallel.cpp -o matrix_padded_parallel -lpthread
g++ matrix_parallel.cpp -o matrix_parallel -lpthread
g++ matrix.cpp -o matrix -lpthread

ocperf.py stat -o matrix.txt -r 1000 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./matrix
ocperf.py stat -o matrix_parallel.txt -r 1000 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./matrix_parallel
ocperf.py stat -o matrix_padded_parallel.txt -r 1000 -e mem_load_uops_l3_hit_retired.xsnp_hitm ./matrix_padded_parallel
