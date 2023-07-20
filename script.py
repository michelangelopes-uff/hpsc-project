from subprocess import call
import os
import sys

executable = ""
if sys.argv[1] == "sequential":
    executable = "./bin/main"
elif sys.argv[1] == "omp":
    executable = "./bin/main_omp"

raw_input_path = "./data/raw_input"
raw_output_path = "./data/raw_output"

for filename in os.listdir(raw_input_path):
    width, height = filename.replace(".raw", "").split("_")[2].split("x")
    print("time", executable, raw_input_path + "/" + filename, raw_output_path + "/" + filename.replace(".raw", ""), width, height)