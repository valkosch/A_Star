# AStar

My first semester project for my Programming 1. class at uni.

The goal of this subject was to introduce us to the C programming language, and I belive I mananged to practise and show the coding skillsI perfected during this course, with this project. My main goal was to make something remarkable and 'real', not some cute project which holds no challenges to me.

This program is a pathfinder on a small rectangle of Budapest, which uses the A* algorithm to find the shortest or fastest path using the streets of Budapest. Every part of the code was written by me.

The data and the map was fetched from OpenStreetMap.

There is also a *makegraph.py* small python code, which I used to fetch the map from OSM then convert it to a graph, which data was separated into two .txt file (nodes and edges).

## How to

### Install

First, clone the repo.

After that you can either use my provided binary to run the program in the terminal with:

```
./astar
```

Or you can build the binary for yourself. To do this first download the dependencies. On my Arch it goes like this, but I'm sure youcan find the same packages with your own package manager.

```
sudo pacman -S sdl2 sdl2_gfx sdl2_image sdl2_ttf
```

Then go to the project directory and build it.

```
gcc *.c -o myexec `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lm
```

### Usage

1. Select 2 distinct points on the map. You can do this with 2 left clicks, on success you will see 2 red dots on the place of the clicks.
2. Select the weight type on the right panel. Red will run A* with time as weight, so the fastest route will be the result, with blue the weight will be distance, so the result is the shortest path. You can select both of them.
3. Click on the GO button.

+ at any time of the duration of the program, you can reset the map, and delete the drawn paths, with a right click.

You can also switch between satellite and normal map view clicking on icon in the right-bottom corner.
