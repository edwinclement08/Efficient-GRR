#!/usr/bin/env python
import graph_transfer_cpp

if __name__ == "__main__":
    # Sample data for our call:
    x, y = 6, 2.3

    answer = graph_transfer_cpp.graph_transfer(x, y)
    print(f"    In Python: int: {x} float {y:.1f} return val {answer:.1f}")
