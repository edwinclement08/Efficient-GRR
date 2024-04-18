""" Task definitions for invoke command line utility for python bindings
    overview article.
"""
import invoke
import sys
import os
import shutil
import glob

on_win = sys.platform.startswith("win")

def print_banner(msg):
    print("==================================================")
    print("= {} ".format(msg))

@invoke.task()
def build_cppmult(c):
    """Build the shared library for the sample C++ code"""
    print_banner("Building C++ Library")
    invoke.run(
        "g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC graph_transfer.cpp "
        "-o libgraph_transfer.so "
    )
    print("* Complete")


def compile_python_module(cpp_name, extension_name):
    invoke.run(
        "g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC "
        "`python -m pybind11 --includes` "
        "-I . "
        "{0} "
        "-o {1}`python3-config --extension-suffix` "
        "-L. -lgraph_transfer -Wl,-rpath,.".format(cpp_name, extension_name)
    )


@invoke.task(build_cppmult)
def build_pybind11(c):
    """Build the pybind11 wrapper library"""
    print_banner("Building PyBind11 Module")
    compile_python_module("graph_transfer_wrapper.cpp", "graph_transfer_cpp")
    print("* Complete")


@invoke.task(build_pybind11)
def test_pybind11(c):
    """Run the script to test PyBind11"""
    print_banner("Testing PyBind11 Module")
    invoke.run("python3 graph_transfer_test.py", pty=True)

@invoke.task
def clean(c):
    """Remove any built objects"""
    for file_pattern in (
        "*.o",
        "*.so",
        "*.obj",
        "*.dll",
        "*.exp",
        "*.lib",
        "*.pyd",
        "cffi_example*",  # Is this a dir?
        "cython_wrapper.cpp",
    ):
        for file in glob.glob(file_pattern):
            os.remove(file)
    for dir_pattern in "Release":
        for dir in glob.glob(dir_pattern):
            shutil.rmtree(dir)


@invoke.task(
    clean,
    build_cppmult,
    build_pybind11,
    test_pybind11,
)
def all(c):
    """Build and run all tests"""
    pass
