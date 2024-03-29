import sys
import os
import subprocess

if len(sys.argv) < 2:
    raise Exception("invalid arguments received")

cnc_path = sys.argv[1]
server_path = os.path.join(cnc_path, "server")
build_path = os.path.join(server_path, "build")

if not os.path.isdir(server_path): 
    raise Exception("invalid server path")

if not os.path.isdir(build_path): 
    os.makedirs(build_path)

subprocess.run(["cmake", ".."], cwd=build_path).check_returncode()

uts = [
    "data_tests",
    "engine_tests",
    "inventory_tests",
    "task_factory_tests",
    "tasks_tests"
]

def find_executable(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)
    raise Exception("exec file " + name + " was not found")

for ut in uts:
    print("building test " + ut)
    subprocess.run(["make", ut], cwd=build_path).check_returncode()
    print("running test " + ut)
    subprocess.run([find_executable(ut, build_path)], cwd=build_path).check_returncode()