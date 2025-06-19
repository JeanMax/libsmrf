# libsmrf

short desc TODO

## RoadMap

* parse missiles
  * display them eventually {smrfhck}

* parse tiles
  * improve display (entrance up/down) {smrfhck}

* parse objects
  * display them (ex: shrine act5)  {smrfhck}

* parse collmaps
  * display them {smrfhck}

* add fakeinputs

* [autolegacy]


* parse player stats:
  * display them (life / xp / ???) {smrfhck}

* [autopot]


* add an abstraction layer to libsmrf {smrfhck}
  * simple units
  * easier to read map?
  * me object that can interact (.click, .move, .menu ...)
* add a plugin system so every [tag]:
  * is a python plugin
  * live in its own thread
  * can access abstraction layer (read only)
  * can access sdk infos
  * can access some user provided python libraries (/lib folder)
  * can be triggered from {smrfhck}
  * can be parsed at runtime
  * can be debugged (logs / backtrace / pdb?)

* [user-friendly plugins]


* add a pather (lot of work to have something nice)

* [follower]


* add item infos to sdk (there's a lot)
* parse items
  * display them {smrfhck}

* add a nip parser (this is going to be a pain in the ass)

* parse inventory (player / npc):
  * display them {smrfhck}

* [shopbot]


* find a proper way to click items on the floor:

* [pickit]


* add shortcuts infos to sdk
* parse shortcuts

* add skills infos to sdk
* parse monsters stats:
  * display them (life / res / ???) {smrfhck}

* [killbot]




## Usage

example link - desc TODO


## Dependencies

* None


## Installation

```shell
make
```
