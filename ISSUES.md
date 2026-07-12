# plasma

plasma works in the simulator but crashes and restarts on the hardware.

# randomness (bounce, fish)

On the hardware, bounce doesn't create random numbers of notes/flagged notes/positions. Each time it runs it looks the same. Same with fish. But, worms does create different numbers of worms each time.

* Again, this is on the hardware. In the simulator they behave as expected.

Comparing them now, One difference I notice is that worms uses a global "STATE" struct which hold the number of worms ("cpus"), whereas bounce and fish use global arrays for number of notes/fish.

# static vars

What (global) vars should be static? Framebuffer arrays? Life arrays?

`ui/Ui.h` doesn't use static, so don't (?)

But fire crashes if framebuffer isn't static, and snow works if it isn't... ?

Fire didn't check bounds for the fb, now it checks, behaves better and can use non-static array.

I wonder what the difference is with bounds-checking and a static array vs. non-static, where the static version works with potentially out of bounds access but the non-static doesn't. Or perhaps (i.e. probably) it's something else I'm not understanding.
