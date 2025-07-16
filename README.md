# MyGame
How to run

Install SDL Libraries

Download SDL3 https://libsdl.org/release/?C=M;O=D

Choose this one: SDL3-devel-3.2.10-VC.zip

Download SDL3_image https://www.libsdl.org/projects/SDL_image/release/?C=M;O=D

Choose this one: SDL3_image-devel-3.2.4-VC.zip

Download SDL3_ttf https://www.libsdl.org/projects/SDL_ttf/release/

Choose this one: SDL3_ttf-devel-3.1.0-VC.zip


Extract all into the same folder for later convenience 

Download the MyGame folder and import everything into Visual Studio 2022 Community Version.

Right click on MyGame

<img width="463" height="813" alt="image" src="https://github.com/user-attachments/assets/504d7088-fbc7-45bf-9a04-3b01a989b048" />

Click Properties

On Configuration Properties

Click on VC++ Directories

On Include Directories (click on the text box, arrow, edit, new line, ...).

Locate the folder where you previously downloaded all the SDL's and include every include folder

On Library Directories (click on the text box, arrow, edit, new line, ...).

Locate the folder where you previously downloaded all the SDL's and include every lib folder and choose x64

On Configuration Properties

Click on Linker->Input

On Additional Dependencies write this: SDL3.lib;SDL3_image.lib;SDL3_ttf.lib

Hit Apply and Run
