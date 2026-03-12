# Dungeon Master
Terminal roguelike game in C

## Build
```bash
docker build -t dungeon-master .
```

## Run
```bash
docker run -it --rm dungeon-master
```

## Goal
    Reach exit (E), collect gold ($), avoid monsters (M).

## Mechanics
- Procedural levels
- Monster follows player
- Collision: monster = -20 HP, gold = +100, exit = next level
- Game over at 0 HP

## Controls
    W/A/S/D - move
    Q - quit
    
## Docker
    Alpine => scratch. Static binary, zero dependencies.

## Image Size
    About 55 KB
