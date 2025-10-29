<div style="text-align: center;">
  <img src="./icon.svg" alt="Touch-Screen-Icon" style="width: 30%; max-width: 100%;">
</div>

# Touch-Screen

## Algoritmo de movimiento
The touch system works with a minimum of six capacitive sensors, arranged in a 2x3 matrix.

This matrix can be expanded modularly depending on how many extra plates you want to add.
It’s important that the total number of plates (vertically) is even, because the algorithm detects “Up” and “Down” movements by recognizing a slide between the top and bottom rows.

An odd number of interconnected plates is required for the system to correctly determine the vertical direction of the cursor movement.
This design choice is made because the ESP32 used in this project has only 8 available touch pins, so the matrix is optimized to make the best use of those resources.

The horizontal detection (Left/Right) algorithm is a bit more complex.
Each group of three plates acts as an analysis region that helps predict the user’s finger movement.
With this setup, it’s not necessary to start sliding from a specific edge, and you can connect any number of plates, either even or odd.

The basic plate connection layout looks like this:

```
1 | 2 | 3
4 | 5 | 6
```

From this base configuration, plates can be interconnected in repeating patterns, for example:

```
1 | 2 | 3 | 4 | 5 | 6
4 | 5 | 6 | 4 | 5 | 6
1 | 2 | 3 | 4 | 5 | 6
4 | 5 | 6 | 4 | 5 | 6
```

In this diagram, the repeated numbers show the connections between touch modules, allowing the system to scale up while keeping the movement detection algorithm consistent.