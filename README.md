# graphing
Primarially for DigiPen MAT300


You will likely need to download and setup wgpu binaries in the extern folders

# prj 2
- Doing bezier curves
- Take user input from clicking on points
  - Spawn with points
- Draw lines between spawned points
- When there are 3 or more points, draw curve
  - Graph bernstein polynomial `gamma(t) = (1-t)^2p0 + 2t(1-t)p1 + t^2p2`
    - Use these as "weights" (they add up to 1!)
  - Use nested linear interpolation 
    - Makes a triangle that calculates gamma
    - Make a slider that moves the shell
  - Also make BB-form
    - That can just make a graph
  - Also midpoint subdivision?