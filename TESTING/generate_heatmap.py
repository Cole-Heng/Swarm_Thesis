# Cole Hengstebeck, hengstcm@miamioh.edu Script to generate a heatmap from a set
# of already generated simulation log files. When displaying analysis window,
# press "Q" to close.
#
# copyright Copyright (c) 2023

from genericpath import exists
from re import T
from sre_constants import SUCCESS
import sys
from subprocess import run
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.collections as collections
import matplotlib.patches as patches
from mpl_toolkits.axes_grid1 import make_axes_locatable
from time import sleep
import argparse
import os


def main():
    # Handle argument parsing
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "num_sims", help="The number of simulation log files to read and process", type=int)
    parser.add_argument(
        "arena_type", help="The arena shape simulated \"open\", \"wall\", or \"bowl\"", type=str)
    # parser.add_argument(
    #     "data_dir", help="The parent directory of a set of log files", type=str)
    args = parser.parse_args()

    death_points = []
    num_alive_global = 0
    num_dead_global = 0
    heatmap = np.zeros((1000, 1000))
    for i in range(args.num_sims):
        num_alive_local = 0
        num_dead_local = 0

        print(f"Reading log file {i}")
        file_name = "./simulation_logs/sim_statistics" + str(i) + ".log"
        stats_file = open(file_name)
        try:
            with open(file_name) as stats_file:
                line = stats_file.readline()
                temp = line.split()
                num_alive_local = num_alive_local + int(temp[1])

                line = stats_file.readline()
                temp = line.split()
                num_dead_local = num_dead_local + int(temp[1])

                line = stats_file.readline()
                temp = line.split()
                num_alive_local = num_alive_local + int(temp[1])

                line = stats_file.readline()
                temp = line.split()
                num_dead_local = num_dead_local + int(temp[1])

                for j in range(3):
                    line = stats_file.readline()

                for j in range(num_dead_local):
                    line = stats_file.readline()
                    temp = line.split()
                    death_points.append((float(temp[0]), float(temp[1])))

                tempHeat = np.loadtxt(stats_file, delimiter=",", skiprows=1)
                heatmap = heatmap + tempHeat
                stats_file.close()

        except IOError as e:
            print(
                f"Couldn't open file. Ensure file naming is of format \"sim_statistics<num>.log\" and data files are in a directory labeled \"simulation_logs\" ({e})")

    #vismap = expand_heatmap(heatmap)
    ax = plt.subplot()
    ax.axis('off')
    
    # grid
    # major_ticks = np.arange(0, 1000, 20)
    # minor_ticks = np.arange(0, 1000, 5)
    # ax.set_xticks(major_ticks)
    # ax.set_xticks(minor_ticks, minor=True)
    # ax.set_yticks(major_ticks)
    # ax.set_yticks(minor_ticks, minor=True)
    # ax.grid(which='minor', alpha=0.2)
    # ax.grid(which='major', alpha=0.5)
    
    plt.box()
    ax.set_xlim(-5, 1005)
    ax.set_ylim(-5, 1005)
    # ax.set_facecolor("skyblue")
    
    im = ax.imshow(heatmap, cmap='viridis_r', interpolation='none', norm=colors.LogNorm())
    
    # starting locations
    xs = np.arange(50, 300, 50)
    ys = np.arange(25, 1025, 50)
    
    for x in xs:
        for y in ys:
          ax.add_patch(patches.Circle((x,y), radius=5, linewidth=0, color='peru'))
    
    wallcolor = 'black'
    
    # walls
    for xi in np.arange(0, 1100, 100):
      ax.add_patch(patches.Rectangle((xi-5,-5),  10, 10, linewidth=0, color=wallcolor))
      ax.add_patch(patches.Rectangle((-5,xi-5),  10, 10, linewidth=0, color=wallcolor))
      ax.add_patch(patches.Rectangle((xi-5,995),  10, 10, linewidth=0, color=wallcolor))
      ax.add_patch(patches.Rectangle((995,xi-5),  10, 10, linewidth=0, color=wallcolor))
    
    # target
    ax.scatter(875, 500, color='magenta', marker="*", s=100)  # marker="s" for square shape

    # obstacles
    if (args.arena_type == "wall"):
        x_wall = [750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750,
                  750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750, 750]
        y_wall = [10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 670,
                  690, 710, 730, 750, 770, 790, 810, 830, 850, 870, 890, 910, 930, 950, 970, 990]
    elif (args.arena_type == "bowl"):
        x_wall = [500, 525, 549, 573, 596, 618, 639, 659, 677, 693, 708, 720, 731, 739, 745, 749,
                  750, 749, 745, 739, 731, 720, 708, 693, 677, 659, 639, 618, 596, 573, 549, 525, 500]
        y_wall = [250, 251, 255, 261, 269, 280, 292, 307, 323, 341, 361, 382, 404, 427, 451, 475,
                  500, 525, 549, 573, 596, 618, 639, 659, 677, 693, 708, 720, 731, 739, 745, 749, 750]

    for (x,y) in zip(x_wall,y_wall):
        ax.add_patch(patches.Rectangle((x-5,y-5), 10, 10, linewidth=0, color=wallcolor))
    
    # deaths
    for x, y in death_points:
        ax.add_patch(patches.Circle((x,y), radius=5, linewidth=0, color='r'))
    
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.25)
    
    plt.colorbar(im, cax=cax)
    
    # plt.show()
    plt.savefig("images/test.png",bbox_inches='tight', dpi=300)

if __name__ == "__main__":
    main()
