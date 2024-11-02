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

<div align="center">
    <img height="400em" src="screenshots/cellular_automata_demo.gif"/>
    <img height="400em" src="screenshots/sand_sim_01.gif"/>
</div>

## Features


<details open>
    <summary>viewport navigation</summary>
    <img height="400em" src="screenshots/cell_move.gif">
    <ul><li>panning</li> <li>exponential zoom</li> <li>multi hotkey navigation</li></li></ul>
</details>



<details open>
    <summary>deleteting cells</summary>
        <img height="400em" src="screenshots/cell_delete.gif">        
</details>


<details open>
<summary>clear canvas</summary>
    <img height="400em" src="screenshots/cell_reset.gif">
</details>

<details open>
    <summary>polling interpolation</summary>
        <img height="400em" src="screenshots/cell_interpolation.gif">
                    
<ul>
    <li>Compensates for low polling rate by interpolating between polling points with Bresenham's line algorithm</li>
    <li>Creates smooth lines during quick mouse movements</li>
</ul>
</details>
<details open>
    <summary>fast enough</summary>
    <img height="400em" src="screenshots/sand_sim_01.gif"/>
    <ul>
      <li>vertex based rendering</li>
      <li>avg 2ms per frame when writing to a >200000 cell canvas</li>
    </ul>
</details>
