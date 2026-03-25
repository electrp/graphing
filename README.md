# graphing

## Executable
Either:
- Go to https://electrp.com/apps/graphing/
- Run the provided executable

## Building
Either:
- Use generate_project.bat to create visual studio project (you may need to run twice)
- Use CMake
- Use the shell.nix and build_em.sh files within wsl to build for web
  - Uses the unstable channel:
  - `nix-shell -I nixpkgs=https://github.com/NixOS/nixpkgs/archive/fef9403a3e4d.tar.gz --command "./build_em.sh"`

## Code locations
Most of the math is contained within `src/Graphing`
