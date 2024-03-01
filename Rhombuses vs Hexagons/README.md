# Rhombuses vs Hexagons

## Overview

This game is a simplification of the famous Plants vs Zombies game. It is written in the GFX Framework.

## Functionality

### Rhombuses
- there are 4 types of rhombuses: orange, blue, yellow and purple.
- the rhombus shop is displayed at the top of the screen. For buying a rhombus, the player must click on the desired color and drag the rhombus to the specific location on the green 3x3 grid.
- each rhombus has a specific cost, specified below the rhombus in the shop. The player starts with a fixed number of coins and can collect more by clicking on the purple stars which appear on the screen at random time intervals. The coins can be spent by purchasing rhombuses.
- once a rhombus is placed, it will start shooting projectiles if a hexagon of the same color is present on the same line as the rhombus.
- if a rhombus and a hexagon collide, the rhombus will die and the hexagon will continue its path.

### Hexagons
- there are 4 types of hexagons: orange, blue, yellow and purple.
- a hexagon spawns on the right side of the screen at random time intervals on one of 3 horizontal lines.
- the hexagon travels from the right side to the left side of the screen. If a hexagon is shot 3 times by a rhombus of the same color, the hexagon will die.
- if the hexagon reaches the 'safe zone' on the left side of the screen a life is lost. The game ends whe 3 lives are lost.

## Sources

This repo contains the GFX Framework. The source files for this game are located in ```src/lab_m1/Tema1/```. The game executable (compiled on M1 processor) is located at ```build/bin/Debug/GFXFramework```.