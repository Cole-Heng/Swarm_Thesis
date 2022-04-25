import sys
import math

def main():
    if (len(sys.argv) != 9):
        print("Usage: create_file_init.py <x of starting point> <y of starting point> <width in pixels> <height in pixels> <placement seperation> <x of wall> <x of target> <y of target>")
        sys.exit()

    x = int(sys.argv[1])
    y = int(sys.argv[2])
    width = int(sys.argv[3])
    height = int(sys.argv[4])
    seperation = int(sys.argv[5])
    x_wall = int(sys.argv[6])
    x_target = int(sys.argv[7])
    y_target = int(sys.argv[8])
    out = open("generated_file_init.txt", "w")

    num_boids = math.trunc(width / seperation) * math.trunc(height / seperation)

    #headers
    out.write(str(num_boids) + "\n")
    out.write(str(2000) + "\n") #num frames
    out.write(str(600) + "\n") #dim size
    out.write(str(2) + "\n") #dimensions
    out.write(str(75) + "\n") #neighbor view rad
    out.write(str(25) + "\n") #neighbor seperation
    out.write(str(75) + "\n") #rule 1, 2, 3
    out.write(str(4) + "\n")
    out.write(str(10) + "\n")

    #boids
    i = 0
    while (i + seperation <= width):
        j = 0
        while(j + seperation <= height):
            out.write(str(x + i) + "\n")
            out.write(str(y + j) + "\n")
            out.write(str(0) + "\n")
            out.write(str(0) + "\n")
            if (i == 0 and j == 0):
                out.write(str(1) + "\n")
            else:
                out.write(str(0) + "\n")
            j += seperation
        i += seperation
    
    #objects
    out.write(str(27) + "\n")
    for i in range(13):
        out.write(str(x_wall) + "\n")
        out.write(str(10 + i * 20) + "\n")
        out.write(str(10) + "\n")
        out.write(str(0) + "\n")
    for i in range(13):
        out.write(str(x_wall) + "\n")
        out.write(str(600 - 10 - i * 20) + "\n")
        out.write(str(10) + "\n")
        out.write(str(0) + "\n")
    out.write(str(x_target) + "\n")
    out.write(str(y_target) + "\n")
    out.write(str(10) + "\n")
    out.write(str(1) + "\n")


    out.flush()
    out.close()
    print("finished")


if __name__ == "__main__":
    main()