## Wrapping C API

- [ ] Colors
  - [X] Simple colors are int constants
  - [ ] patterns (pointers to a pattern)
- [ ] Input handling
  - [x] Button presses
  - [ ] Crank support
- [ ] Image/Image Table Handling
  - [x] Load bitmap
  - [x] Load Bitmap Table
  - [x] Get specific Bitmap from Table
  - [ ] Size of image table
- [ ] Tilemap support
  - [ ] new/free
  - [ ] setImageTable +get
  - [ ] setSize (in tiles)
  - [ ] getPixelSize (maybe unnecessary?)
  - [ ] setTiles - each a uint tile identifier (into the imageTable:getBitmap)  
        note this also sets size in tiles
  - [ ] setTileAtPosition +get
  - [ ] drawAtPoint (draws whole bitmap)
- [ ] Sprites
  - [x] Set image 
  - [x] moveTo
  - [x] Creation
  - [x] Draw all sprites
  - [x] Custom draw function
  - [~] ~Custom update function~ thinking maybe this lives Ruby side better than C
  - [x] set size 
  - [ ] set bounds
  - [ ] Set collision rect
  - [ ] set center
  - [ ] Collisions
- [ ] JSON support
- [ ] Font support: font as a class first?
- [ ] Strings support (lookup tables): playdate->system->getLocalizedText


## Creating useful wrappers

- [ ] Work out a full sprite class with animation state helpers (blink/walk) and inputs to move around
  - Animation timers - loops?
- [ ] LDTK support
- [ ] Animation loop helpers
  - Think about maybe making timers scene specific?
- [ ] UI Helpers
  - [ ] Menu
    - [ ] Probably needs nine-slice support
    - [ ] Maybe needs gridview helper?
  - [ ] Dialog  
    - [ ] Probably needs nine-slice


## Possible fixes/changes

- UI stuff should pause the main scene - should scenes be a _stack_ - allowing multiple scenes to draw? (Then a menu / dialog is a scene pushed on top of another scene)


## Build process improvements

With cmake simulator vs device builds, artifacts of one or the other sometimes leak over. nmake clean is required to safely avoid. We probably need to ensure the pdex files built into `Source` are wiped on every build? Ideally we can build fresh device or simulator build from same repo at some point?
