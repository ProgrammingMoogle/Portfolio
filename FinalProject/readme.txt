For your convenience, this zip-file contains a folder called 'include' that contains the header files of all the packages that you are allowed to use for this assignment.  However, DO NOT assume that when I compile your code on my machine that these files are located in a subfolder with the name 'include' -- they will not be.  This means that you should write, for instance,
    #include "Affine.h"
in your code.  You should NOT write:
    #include "include\Affine.h"

If you compile from the command line, you will need to tell the compiler where to look for the include files.  E.g., from the Visual Studio command prompt you would type:
  cl /EHsc /Iinclude /DCLIP_PIXELS cs250final_proj.cpp DrawFrame.cpp
     cs250_proj2-clip_pixels.lib opengl32.lib glew32.lib sdl2.lib sdl2main.lib
     /link /subsystem:console
Note that your final submission should also work without pixel-level clipping:
  cl /EHsc /Iinclude cs250final_proj.cpp DrawFrame.cpp
     cs250_proj2.lib opengl32.lib glew32.lib sdl2.lib sdl2main.lib
     /link /subsystem:console

