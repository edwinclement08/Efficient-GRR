#!/usr/bin/env python
import resolver_cpp

if __name__ == "__main__":
    # Sample data for our call:
    x, y = 6, 2.3

    answer = resolver_cpp.resolver(x, y)
    print(f"    In Python: int: {x} float {y:.1f} return val {answer:.1f}")
