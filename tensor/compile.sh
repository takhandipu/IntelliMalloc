# g++ -std=c++11 -I /home/takh/eigen/ -DEIGEN_USE_THREADS -DMSIZE=100 tensor_parallel.cpp -lpthread -o execs/1_100
for (( c=100; c<=2000; c+=100 ))
do
	g++ -std=c++11 -I /home/takh/eigen/ -DEIGEN_USE_THREADS -DMSIZE=$c tensor_parallel.cpp -lpthread -o execs/$1_$c
done

