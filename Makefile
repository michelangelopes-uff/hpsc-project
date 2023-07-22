compile:
	@echo "compiling..."
	@g++ src/main.c src/filter.c src/image.c -o bin/main -lm
	@g++ src/main.c src/filter.c src/image.c -o bin/main_o3 -lm
	@g++ src/main.c src/filter.c src/image.c -o bin/main_omp -fopenmp -lm
	@g++ src/main.c src/filter.c src/image.c -o bin/main_omp_o3 -fopenmp -lm -O3
	@g++ src/main_opencl.cpp src/image.c -o bin/main_opencl -lOpenCL

prepare_run_script:
	@echo "preparing..."
	@python3 script.py sequential > script.sh

run:
	@echo "running..."
	@./bin/main

run_o3:
	@echo "running..."
	@./bin/main_o3

run_omp:
	@echo "running..."
	@./bin/main_omp

run_omp_o3:
	@echo "running..."
	@./bin/main_omp_o3

run_opencl:
	@echo "running..."
	@./bin/main_opencl

convert_raw:
	@python3 image_converter.py raw

convert_png:
	@python3 image_converter.py png