<div align="center">
    <h1>Cellular Automata</h1>
    <div>
      <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
      <img src="https://img.shields.io/badge/SFML-8CC445?style=for-the-badge&logo=sfml&logoColor=white">
    </div>
</div><br>

I started this project with the intent to **explore cellular automata**.
While working my focus transitioned from modeling specific cell behavior to creating a structure to facilitate arbitrary cell behavior.<br><br>
The result is a **framework for manipulating and viewing cellular** interactions on a 2D grid. Specifically, a basic falling sand simulation.

> This is my first C++ project and is pretty barebones, but it provides a satisfactory starting off point for visualizing animated cells.

## Features
- viewport navigation
  - panning
  - exponential zoom
  - multi hotkey navigation
- fast enough
  - vertex based rendering
  - avg 2ms per frame when writing to a >200000 cell canvas

## Screenshots
<div align="center">
    <img height="400em" src="cellular_automata/screenshots/cellular_automata_demo.gif"/>
    <img height="400em" src="cellular_automata/screenshots/sand_sim_01.gif"/>
</div>
