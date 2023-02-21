# Cole Hengstebeck, hengstcm@miamioh.edu
# Script to run a number of boid simulations back-to-back and present cumulative
# data about the simulations. When displaying analysis window, press "Q" to close.
# version 1.0
# date 2022-08-17
#
# copyright Copyright (c) 2022

from genericpath import exists
from re import T
from sre_constants import SUCCESS
import sys
from subprocess import run
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from mpl_toolkits.axes_grid1 import make_axes_locatable
from time import sleep
import argparse
import os


def main():
    # Handle argument parsing
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "num_sims", help="The number of simulations to run", type=int)
    parser.add_argument(
        "init_file", help="The init file for each simulation to use", type=str)
    parser.add_argument(
        "-L", "--log_all", help="Define if the log file for all simulations should be kept. Otherwise only the last simulation is kept", action="store_true")
    parser.add_argument("-S", "--suppress_analysis",
                        help="Run simulations but don't show analysis graphs for debug purposes", action="store_true")
    args = parser.parse_args()

    # Initialize dictionaries for simulation data
    file_in = open(args.init_file, "r")
    num_boids = file_in.readline()
    while (num_boids[0] == "#"):
        num_boids = file_in.readline()  # skip comment lines
    num_boids = int(num_boids)
    file_in.close()
    trial_number = [i for i in range(args.num_sims)]
    swarm_achieved = [0] * args.num_sims
    swarm_broken = [False] * args.num_sims
    success_and_alive = [0] * args.num_sims
    success_and_dead = [0] * args.num_sims
    fail_and_alive = [0] * args.num_sims
    fail_and_dead = [0] * args.num_sims
    success_frame = [0] * args.num_sims
    elapsed_time = [0] * args.num_sims
    death_points = []
    heatmap = np.zeros((1000, 1000))

    # Make a dir to save all simulation logs and clear it
    run(["rm", "-r", "./simulation_logs/"])
    if not os.path.exists("./simulation_logs/"):
        os.mkdir("simulation_logs")

    run(["cp", args.init_file, "../" + args.init_file])

    # Run simulations with or without saving all the log files
    if args.log_all:
        for i in range(args.num_sims):
            # call_out = "../boids_sim FILE_INIT " + \
            #     str(args.init_file) + \
            #     " ./simulation_logs/sim_out{}.log {}".format(i, i)
            # print("Calling: " + call_out)
            run(["../boids_sim", "FILE_INIT",
                str(args.init_file), "./simulation_logs/sim_out{}.log".format(i), "{}".format(i)])
            sleep(0.1)
            stats = parse_sim_stats()
            swarm_achieved[i] = int(stats[0])
            swarm_broken[i] = stats[1]
            success_and_alive[i] = int(stats[2])
            success_and_dead[i] = int(stats[3])
            fail_and_alive[i] = int(stats[4])
            fail_and_dead[i] = int(stats[5])
            success_frame[i] = int(stats[6])
            elapsed_time[i] = float(stats[7])
            stats_file = open("./sim_statistics.log")
            for j in range(7):
                line = stats_file.readline()
            for j in range(fail_and_dead[i] + success_and_dead[i]):
                line = stats_file.readline()
                temp = line.split()
                death_points.append((float(temp[0]), float(temp[1])))
            tempHeat = np.loadtxt(stats_file, delimiter=",", skiprows=1)
            heatmap = heatmap + tempHeat
            stats_file.close()
            run(["cp", "./sim_statistics.log",
                "./simulation_logs/sim_statistics{}.log".format(i)])
            sleep(0.1)
    else:
        for i in range(args.num_sims):
            # call_out = "../boids_sim FILE_INIT " + \
            #     str(args.init_file) + \
            #     " ./simulation_logs/sim_out.log {}".format(i)
            # print("Calling: " + call_out)
            run(["../boids_sim", "FILE_INIT",
                 str(args.init_file), "./simulation_logs/sim_out.log {}".format(i)])
            sleep(0.1)
            stats = parse_sim_stats()
            swarm_achieved[i] = int(stats[0])
            swarm_broken[i] = stats[1]
            success_and_alive[i] = int(stats[2])
            success_and_dead[i] = int(stats[3])
            fail_and_alive[i] = int(stats[4])
            fail_and_dead[i] = int(stats[5])
            success_frame[i] = int(stats[6])
            elapsed_time[i] = float(stats[7])
            stats_file = open("./sim_statistics.log")
            for j in range(7):
                line = stats_file.readline()
            for j in range(fail_and_dead[i] + success_and_dead[i]):
                line = stats_file.readline()
                temp = line.split()
                death_points.append((float(temp[0]), float(temp[1])))
            tempHeat = np.loadtxt(stats_file, delimiter=",", skiprows=1)
            heatmap = heatmap + tempHeat
            stats_file.close()
            sleep(0.1)

    # Display all analysis plots
    if not args.suppress_analysis:
        labels = 'Never achieved', 'Stayed Together', 'Broke'
        never_achieved = 0
        num_broke = 0
        for i in swarm_achieved:
            if (i == -1):
                never_achieved += 1
        for i in swarm_broken:
            if (i == True):
                num_broke += 1
        sizes = [100 * never_achieved / args.num_sims, 100 *
                 (args.num_sims - never_achieved - num_broke) / args.num_sims, 100 * num_broke / args.num_sims]
        explode = (0, 0.1, 0)

        fig, axs = plt.subplots(2, 3, constrained_layout=True)
        axs[0, 0].pie(sizes, explode=explode, labels=labels,
                      autopct="%1.1f%%", shadow=True, startangle=90)
        axs[0, 0].set_title("Swarm Statuses")

        num_success = [success_and_alive[x] + success_and_dead[x]
                       for x in range(len(success_and_alive))]
        axs[0, 1].hist(num_success)
        axs[0, 1].set_title("Number of Successful Boids")
        axs[0, 1].set_xlabel("Number of Boids (out of " + str(num_boids) + ")")
        axs[0, 1].set_ylabel("Number of Simulations")

        num_alive = [success_and_alive[x] + fail_and_alive[x]
                     for x in range(len(success_and_alive))]
        axs[0, 2].hist(num_alive)
        axs[0, 2].set_title("Number of Boids Alive")
        axs[0, 2].set_xlabel("Number of Boids (out of " + str(num_boids) + ")")
        axs[0, 2].set_ylabel("Number of Simulations")

        # axs[1, 0].hist(swarm_achieved, 20)
        # axs[1, 0].set_title("Frame swarm was achieved")
        # axs[1, 0].set_xlabel("Frame Number (-1 = never)")
        # axs[1, 0].set_ylabel("Number of Simulations")

        axs[1, 0].hist(swarm_achieved, 20)
        axs[1, 0].set_title("Frame swarm was achieved")
        axs[1, 0].set_xlabel("Frame Number (-1 = never)")
        axs[1, 0].set_ylabel("Number of Simulations")

        labels = 'Success and Alive', 'Success and Dead', 'Fail and Alive', 'Fail and Dead'
        sizes = [sum(success_and_alive), sum(success_and_dead),
                 sum(fail_and_alive), sum(fail_and_dead)]
        explode = (0.1, 0, 0, 0)
        axs[1, 1].pie(sizes, explode=explode, labels=labels,
                      autopct="%1.1f%%", shadow=True, startangle=90)
        axs[1, 1].axis('equal')
        axs[1, 1].set_title("Sum of all Boid Statuses (" + str(num_boids *
                                                               args.num_sims) + " Total Boids Simulated)")

        axs[1, 2].plot(elapsed_time)
        axs[1, 2].set_title("Elapsed time")
        axs[1, 2].set_xlabel("Sim number\nAvg: " + str(np.mean(elapsed_time)))
        axs[1, 2].set_ylabel("Time (s)")

        plt.suptitle("Swarm Simulation Analysis. {} Simulations Run".format(
            args.num_sims), fontweight='bold')
        mng = plt.get_current_fig_manager()
        mng.full_screen_toggle()
        plt.show()

        #vismap = expand_heatmap(heatmap)
        ax = plt.subplot()
        im = ax.imshow(heatmap, cmap='hot',
                       interpolation='none', norm=colors.LogNorm())
        for x, y in death_points:
            ax.scatter([x], [y], c='b', marker='x', s=10)
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.1)
        plt.colorbar(im, cax=cax)

        plt.show()


# Take heatmap of individual boid location points and expand it to a map of
# visibility based on a visibility radius of 50
def expand_heatmap(heatmap):
    visibility_map = np.zeros((1000, 1000))
    for x in range(1000):
        print(x)
        for y in range(1000):
            if (heatmap[y][x] == 0):
                continue
            for r in range(max(0, y - 50), min(1000, y + 50)):
                for c in range(max(0, x - 50), min(1000, x + 50)):
                    visibility_map[r][c] += heatmap[y][x]
    return visibility_map


def parse_sim_stats():
    swarm_achieved = 0
    swarm_broke = False
    success_and_alive = 0
    success_and_dead = 0
    fail_and_alive = 0
    fail_and_dead = 0
    num_success = 0
    num_alive = 0
    success_frame = 0
    elapsed_time = 0
    achieved_set = False
    file_in = open("./sim_statistics.log", "r")
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
        elif (temp[0] == "success_frame:"):
            success_frame = temp[1]
        elif (temp[0] == "elapsed_time:"):
            elapsed_time = temp[1]
        elif (temp[0] == "Heatmap:"):
            break
        elif (temp[0] == "deaths:"):
            break
        else:
            print("WARNING: Unknown line read from simulation stats log:")
            print(temp[0])

        line = file_in.readline()

    if (achieved_set == False):
        swarm_achieved = -1
    file_in.close()
    return [swarm_achieved, swarm_broke, success_and_alive, success_and_dead, fail_and_alive, fail_and_dead, success_frame, elapsed_time]


if __name__ == "__main__":
    main()
