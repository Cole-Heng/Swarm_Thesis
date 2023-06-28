# Swram Robotics Thesis
#### Cole Hengstebeck (Hengstcm@miamioh.edu)
## Advisors
#### Dr. Peter Jamieson (Jamiespa@miamioh.edu)
#### Dr. Bryan Van Scoy (Bvanscoy@miamioh.edu)

---

## Introduction
This thesis project looks into the control of robot swarms. Using the simple Biods algorithm developed by Craig Reynolds as a base, the simulation emulates the flocking behavior of these Boid objects. Using extensions of the Boid control rules, object avoidance and goal seeking can be added. This is one area of exploration in this thesis. The Boids are controlled by 3 rules based on the Boids within their field of view as discussed below

1. **Separation** - Boids try to maintain a distance from each other (avoid collision)
2. **Alignment** - Boids align their heading with the average heading of the other boids they can see (move together).
3. **Cohesion** - Boids tend towards the center of mass of all other Boids in view (form a flock).

Each of these rules has a weight assigned to it to influence the behavior of the Boids. Other settings, such as the radius around a boid in which it can "see" other Boids, affect the emergent behavior of the Boids.

### Common Terms

- Swarm - A collection of agents that have individual control but have a collective goal and interact with one another
- Biod - An agent that follows the three flocking rules of a Boid
- Ghost Boid - A Boid that is "attached" to a Boid or an object but that does not move and does not cause collisions
- Leader Boid - A Boid that has the role of Leader. They are assigned a ghost boid that always points towards the waypoint.
- Object - Points in space. These are either a waypoint (an object the boids are trying to reach) or an obstacle. Waypoints do not get assigned IDMs but all other objects do.
- ATON - Aid TO Navigation. These are Ghost Boids that surround objects and form the perimeter wall so Boids avoid them.
- Cardinal Marks - AKA CMs are ATONs that form the perimeter wall
- Isolated Danger Marks - AKA IDMs are ATONs that surround an obstacle, one in each of the four cardinal directions around an object.
  

---

## Building


To select between simulating using Control Barrier Functions or not,
modify `./SRC/types.h` to define `USE_CBF`.


Run the following in the main directory

For Debug `cmake -DCMAKE_BUILD_TYPE=Debug`
For Release `cmake -DCMAKE_BUILD_TYPE=Release`

Then `make && make install`

---

## Running

There are two main methods to run simulations. It is possible to run a single simulation or run a script that runs multiple simulations back to back and displays an analysis of the simulations' performance.  Running a simulation does not have a GUI or animation. Instead, a simulation produces a log file that defines all the data and movements that occurred during the simulation. This log file can then be read and processed to show the GUI and animation.

### Running One Simulation

There are two modes to initialize the starting state of the simulation. A random initialization where the program positions Boids in random locations, or a file initialization where the user provides a file defining the starting positions of the boids. Run one of the following commands with the appropriate parameters.

Random Initialization
```bash
./boids_sim RAND_INIT <num_boids> <num_frames> <dimension_size> <num_objects> <log_file_name> <RNG_seed>
```

File Initialization
```bash
./boids_sim FILE_INIT <init_file> <log_file_name> <RNG_seed>
```
<br>

Parameters
- **num_boids** - The number of boids to be created
- **num_frames** - The number of frames to simulate, higher means a longer simulation
- **dimension_size** - The pixel dimensions of the simulation window. The window is always square, aka NxN
- **num_objects** - The number of objects to be created. Boids try to avoid collision with these
- **log_file_name** - The name of the log file the simulation will write to for animation playback later
- **init_file** - The name (including extension like `.txt`) of the file that holds all the initialization data
- **RNG_seed** - A positive integer number or zero to be the seed for `srand()` function or a
  negative number to seed using clock time (aka random seed)


### Running Multiple Simulations

This script works by automatically calling the file initialization command from above. Therefore this script requires an initialization file. This script has some optional arguments that can be added if desired. After running, any simulation logs will be saved in `./TESTING/simulation_logs` and a plot of all analysis graphs will be displayed

**Important** To close out of the fullscreen analysis window, press the `Q` key

```bash
cd ../TESTING
python3 run_simulations.py <num_simulations> <init_file> [-L] [-S]
```
<br>

Parameters
- **num_simulations** - The number of back-to-back simulations to run. Each run is independent of the others.
- **init_file** - The name (including extension like `.txt`) of the file that holds all the initialization data
- **-L** - (Optional) If selected, all log files will be saved, otherwise only the log file of the most recent simulation will be saved.
- **-S** - (Optional) If selected, the analysis will be suppressed and no analysis graphs will be shown.

### Running GUI and Animation

To playback a log file to see the animation, Processing needs to be downloaded from [here](https://processing.org/download). Processing is an IDE and language to help make animations. After it is downloaded and installed, open `PROCESSING_BOIDS_VIEWER_2D.pde`located in `./TESTING/PROCESSING_BOIDS_VIEWER_2D`. Once open, click the play button at the top left of the window. Lastly, select and open the log file you wish to play.

---

## Creating Initialization Files

### File From Scratch

To create a file from scratch, 3 sections must be written, the Header, the
Boids, and the Objects. Each parameter is on a new line and there are no blank
lines in the file. An example init file can be found at
`./TESTING/test_init.txt`. The structure of parameters in the file is below and
many of the definitions of the parameters can be found from commands in above
sections. All values should be `int`

**Note** Comment lines can be added to the very top of an init file by starting
a line with `#`. Comments cannot be put anywhere other than the start, but any
number of comment lines can be added
**WARNING** If you use comment lines, each line has a maximum of 100 characters,
otherwise confusing runtime errors occur

#### Header

- num_boids
- num_frames
- dimension_size
- dimensions - 2 or 3 for 2-dimensional or 3-dimensional
- neighbor_radius - The radius around a Boid in which it can see other Boids
- neighbor_seperation - The distance Boids try to keep from each other
- seperation_weight
- alignment_weight
- cohesion_weight

#### Boids

- x-position
- y-position
- x-velocity
- y-velocity (if both x and y velocities are zero, then the boid points in a random direction to start)
- leader-status - 1 to make it a leader boid, 0 to make it a regular boid
Repeat the above parameters for every boid

#### Objects

- num_objects
- x-position
- y-position
- radius - The size of the object
- waypoint_status - 1 to make this object a goal for the boids, 0 to make it a normal obstacle

### File Using Script

Instead of writing by hand, `create_file_init.py` can make a basic initialization file with a few provided parameters. The script creates a grid of boids by the user providing the x and y coordinate of the top-left corner of the grid, the width and height of the grid in pixels, and the pixel seperation of the boids. The number of boids that can fit in the space is calculated and the positions for the Boids is writen to the file, all with random initial headings. A verticle wall of Objects can also be placed by specifying the x-cordinate of the wall. The wall will havea pre-generated hole for the Boids to move through. Lastly, a the x and y coordinates of a waypoint can be specified to generate one.

```bash
cd ./TESTING
python3 create_file_init.py <x_of_grid> <y_of_grid> <width_of_grid (pixels)> <height_of_grid (pixels)> <Boid seperation (pixels)> <x_of_wall> <x_of_waypoint> <y_of_waypoint>
```

This will create a file named `generated_file_init.txt` which can be used for simulations.

---

### Generate heatmaps from logged data

If you already have data logged and stored from running simulations, you can
generate heatmaps from the log files. All simulation statistics files must have
the naming convention `sim_statistics<num>.log` where `<num>` is the simulation
number starting from 0. All files must be stored in the directory
`./TESTING/simulation_logs`. Run the following to generate a heatmap:

```bash
cd ../TESTING
python3 generate_heatmap.py <num_simulations> <arena_type>
```
<br>

Parameters
- **num_simulations** - The number of simulations with log files to read and process.
- **arena_type** - The type of arena simulated (used to display obstacles on the
  heatmap). Options are `open`, `wall`, or `bowl`.

---

