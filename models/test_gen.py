#!/usr/bin/env python

class Point(object):
    def __init__(self, x, y, z) -> None:
        self.x = x
        self.y = y
        self.z = z

    def __imul__(self, v):
        self.x *= v
        self.y *= v
        self.z *= v
        return self

class Cube(object):
    def __init__(self, base_scale) -> None:
        self.points = list()
        self.points.append(Point(-1, -1, -1))
        self.points.append(Point(1, -1, -1))
        self.points.append(Point(-1, 1, -1))
        self.points.append(Point(1, 1, -1))
        self.points.append(Point(-1, -1, 1))
        self.points.append(Point(1, -1, 1))
        self.points.append(Point(-1, 1, 1))
        self.points.append(Point(1, 1, 1))
        for point in self.points:
            point *= base_scale

    def write(self, fd, ind):
        for point in self.points:
            fd.write(f"v {point.x} {point.y} {point.z}\n")
        fd.write(f"f {ind + 1} {ind + 2} {ind + 4} {ind + 3}\n")
        fd.write(f"f {ind + 5} {ind + 6} {ind + 8} {ind + 7}\n")
        fd.write(f"f {ind + 1} {ind + 5} {ind + 6} {ind + 2}\n")
        fd.write(f"f {ind + 3} {ind + 7} {ind + 8} {ind + 4}\n")

class Alt(object):
    def __init__(self, base_scale) -> None:
        self.points = list()
        self.points.append(Point(-1, 0, 0))
        self.points.append(Point(0, -1, 0))
        self.points.append(Point(0, 0, -1))
        self.points.append(Point(1, 0, 0))
        self.points.append(Point(0, 1, 0))
        self.points.append(Point(0, 0, 1))
        for point in self.points:
            point *= base_scale

    def write(self, fd, ind):
        for point in self.points:
            fd.write(f"v {point.x} {point.y} {point.z}\n")
        fd.write(f"f {ind + 1} {ind + 2} {ind + 4} {ind + 5}\n")
        fd.write(f"f {ind + 2} {ind + 3} {ind + 5} {ind + 6}\n")
        fd.write(f"f {ind + 1} {ind + 3} {ind + 4} {ind + 6}\n")

def generate(max_size):
    curr_size = 0
    curr_base = 1
    file = open(f"generated_{max_size}.obj", mode='w')
    while curr_size < max_size:
        obj = Alt(curr_base)
        obj.write(file, curr_size)
        curr_size += len(obj.points)
        obj = Cube(curr_base)
        obj.write(file, curr_size)
        curr_size += len(obj.points)
        curr_base += 1
    file.close() 

def main():
    generate(1000)
    generate(1000000)

if __name__ == "__main__":
    main()