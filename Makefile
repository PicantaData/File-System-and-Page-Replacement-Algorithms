all:
	gcc -o vmsim vmsim.cpp -lstdc++
	gcc -o vmgen vmgen.cpp -lstdc++
	gcc -o vmstats vmstats.cpp -lstdc++