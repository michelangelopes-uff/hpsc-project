compile:
	@echo "compiling..."
	@g++ src/main.c src/filter.c src/image.c -o bin/main -lm
	@g++ src/main.c src/filter.c src/image.c -o bin/main_omp -fopenmp -lm
	@g++ main.cpp -o bin/main_opencl -lOpenCL

prepare_run_script:
	@echo "preparing..."
	@python3 script.py sequential > script.sh

run:
	@echo "running..."
	@python3 script.py sequential > script.sh
	@bash script.sh >> measures/time.txt 2>&1

run_omp:
	@echo "running..."
	@python3 script.py omp > script_omp.sh
	@bash script_omp.sh >> measures/time_omp.txt 2>&1

run_opencl:
	@./bin/main_opencl

convert_raw:
	@python3 image_converter.py raw

convert_png:
	@python3 image_converter.py png