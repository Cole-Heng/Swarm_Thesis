from re import T
from sre_constants import SUCCESS
import sys
from subprocess import call
import numpy as np
import matplotlib.pyplot as plt
from time import sleep

def main():
    if (len(sys.argv) != 3):
        print("Usage: run_simulations.py <# of simulations to run> <path to init file>")
        sys.exit()
    # call(["cd", "../"])
    # call(["make"])
    # call(["cd", "TESTING/"])
    file_in = open(str(sys.argv[2]), "r")
    num_boids = int(file_in.readline())
    file_in.close()
    trial_number = [i for i in range(int(sys.argv[1]))]
    swarm_achieved = [0] * int(sys.argv[1])
    swarm_broken = [False] * int(sys.argv[1])
    success_and_alive = [0] * int(sys.argv[1])
    success_and_dead = [0] * int(sys.argv[1])
    fail_and_alive = [0] * int(sys.argv[1])
    fail_and_dead = [0] * int(sys.argv[1])
    for i in range(int(sys.argv[1])):
        call(["../boids_sim", "FILE_INIT", str(sys.argv[2]), "sim_out.log"])
        sleep(0.5)
        stats = parse_sim_stats()
        swarm_achieved[i] = int(stats[0])
        swarm_broken[i] = stats[1]
        success_and_alive[i] = int(stats[2])
        success_and_dead[i] = int(stats[3])
        fail_and_alive[i] = int(stats[4])
        fail_and_dead[i] = int(stats[5])
        sleep(0.5)


        # print(i)
        # sleep(0.5)
        # swarm_achieved[i] = get_first_frame_of_swarm(num_boids)
        # sleep(0.5)

        

    # print(str(trial_number))
    # print(str(swarm_achieved))

    labels = 'Never achieved', 'Stayed Together', 'Broke'
    never_achieved = 0
    num_broke = 0
    for i in swarm_achieved:
        if (i == -1):
            never_achieved += 1
    for i in swarm_broken:
        if (i == True):
            num_broke += 1
    sizes = [100 * never_achieved / int(sys.argv[1]), 100 * (int(sys.argv[1]) - never_achieved - num_broke) / int(sys.argv[1]), 100 * num_broke / int(sys.argv[1])]
    explode = (0, 0.1, 0)

    fig1, ax1 = plt.subplots()
    ax1.pie(sizes, explode=explode, labels=labels, autopct="%1.1f%%", shadow=True, startangle=90)
    ax1.axis('equal')  # Equal aspect ratio ensures that pie is drawn as a circle.
    plt.title("Swarm Statuses")
    plt.show()

    num_success = success_and_alive + success_and_dead
    plt.hist(num_success)
    plt.title("Number of Successful Boids")
    plt.xlabel("Number of Boids (out of " + str(num_boids) + ")")
    plt.ylabel("Number of Simulations")
    plt.show()

    num_alive = success_and_alive + fail_and_alive
    plt.hist(num_alive)
    plt.title("Number of Boids Alive")
    plt.xlabel("Number of Boids (out of " + str(num_boids) + ")")
    plt.ylabel("Number of Simulations")
    plt.show()

    plt.hist(swarm_achieved, 20)
    plt.title("Frame swarm was achieved")
    plt.xlabel("Frame Number (-1 = never)")
    plt.ylabel("Number of Simulations")
    plt.show()

    labels = 'Success and Alive', 'Success and Dead', 'Fail and Alive', 'Fail and Dead'
    sizes = [sum(success_and_alive), sum(success_and_dead), sum(fail_and_alive), sum(fail_and_dead)]
    explode = (0.1, 0, 0, 0)
    fig1, ax1 = plt.subplots()
    ax1.pie(sizes, explode=explode, labels=labels, autopct="%1.1f%%", shadow=True, startangle=90)
    ax1.axis('equal')  # Equal aspect ratio ensures that pie is drawn as a circle.
    plt.title("Sum of all Boid Statuses (" + str(num_boids * int(sys.argv[1])) + " Total Boids Simulated)")
    plt.show()


def parse_sim_stats():
    swarm_achieved = 0
    swarm_broke = False
    success_and_alive = 0
    success_and_dead = 0
    fail_and_alive = 0
    fail_and_dead = 0
    num_success = 0
    num_alive = 0
    achieved_set = False
    file_in = open("sim_statistics.log", "r")
    line = file_in.readline()
    while (line):
        temp = line.split()
        if (temp[0] == "swarm_achieved:"):
            if (achieved_set == False):
                achieved_set = True
                swarm_achieved = int(temp[1])
        elif (temp[0] == "swarm_broken:"):
            swarm_broke = True
        elif (temp[0] == "success_and_alive:"):
            success_and_alive = temp[1]
        elif (temp[0] == "success_and_dead:"):
            success_and_dead = temp[1]
        elif (temp[0] == "fail_and_alive:"):
            fail_and_alive = temp[1]
        elif (temp[0] == "fail_and_dead:"):
            fail_and_dead = temp[1]
        else:
            print("WARNING: Unknown line read from simulation stats log:")
            print(temp[0])

        line = file_in.readline()

    if (achieved_set == False):
        swarm_achieved = -1
    file_in.close()
    return [swarm_achieved, swarm_broke, success_and_alive, success_and_dead, fail_and_alive, fail_and_dead]


def get_first_frame_of_swarm(num_boids):
    file_in = open("swarm_count.log", "r")
    i = 0
    frame = int(file_in.readline())

    for line in file_in:
        #print(str(line))
        if (int(line) == num_boids):
            file_in.close()
            print(i)
            return i
        i += 1
    file_in.close()
    return -1

if __name__ == "__main__":
    main()