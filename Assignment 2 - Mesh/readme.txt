* Place 'SDL2.dll' and 'glew32.dll' in:
      C:\WINDOWS

* Place 'SDL2.lib', 'SDL2main.lib', and 'glew32.lib' in:
      C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib

* Place the folders (and their contents) 'SDL2' and 'GL' in:
      C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include


If you are working on a DigiPen machine (and don't have administrative
privileges), you can do the following.  In the folder that you are working in:
* Place copies of 'SDL2.lib', 'SDL2main.lib', 'SDL2.dll', 'glew32.lib', and
  'glew32.dll' in the current folder.
* Place copies of the folders (and their contents) 'SDL2' and 'GL' in the
  current folder.
* Compile your code from the Visual Studio 2015 Command Prompt using
      cl /EHsc /I. <file(s)> SDL2.lib SDL2main.lib opengl32.lib glew32.lib /link /subsystem:console

