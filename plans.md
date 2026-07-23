## Wrapping C API

- [ ] Colors
  - [X] Simple colors are int constants
  - [ ] patterns (pointers to a pattern)
- [ ] Input handling
- [ ] Image/Image Table Handling
  - [x] Load bitmap
  - [x] Load Bitmap Table
  - [x] Get specific Bitmap from Table
- [ ] Sprites
  - [x] Set image 
  - [x] moveTo
  - [x] Creation
  - [x] Draw all sprites
  - [ ] Custom draw function
  - [ ] Custom update function
  - [ ] set size / bounds
  - [ ] Collisions


## Creating useful wrappers

- [ ] Work out a full sprite class with animation state helpers (blink/walk) and inputs to move around
  - Work out inputs
  - Image table support
  - Probably support for update vs draw on sprites

## API Coverage

* Bitmap
  - Loading a bitmap
* Graphics
  - drawText
  - clear (no color yet)
* System
  - logToConsole, FPS
