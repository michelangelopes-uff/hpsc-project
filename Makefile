compile:
	@echo "compiling..."
	@g++ -lm src/main.c src/filter.c src/image.c -o bin/main
	
run:
	@echo "running..."
	./bin/main $(filename)