# MyGame
How to run

Install SDL Libraries

Download SDL3 https://libsdl.org/release/?C=M;O=D

--- Choose this one: SDL3-devel-3.2.10-VC.zip

Download SDL3_image https://www.libsdl.org/projects/SDL_image/release/?C=M;O=D

--- Choose this one: SDL3_image-devel-3.2.4-VC.zip

Download SDL3_ttf https://www.libsdl.org/projects/SDL_ttf/release/

--- Choose this one: SDL3_ttf-devel-3.1.0-VC.zip


Extract all into the same folder for later convenience 

Download the MyGame folder and clone everything into Visual Studio 2022 Community Version.
<img width="364" height="145" alt="image" src="https://github.com/user-attachments/assets/13693a61-20ba-4257-9d10-75579ca80ac1" />

Right click on MyGame

<img width="463" height="813" alt="image" src="https://github.com/user-attachments/assets/504d7088-fbc7-45bf-9a04-3b01a989b048" />

Click Properties and On Configuration Properties

-- Click on VC++ Directories

-- -- On Include Directories (click on the text box, arrow, edit, new line, ...)
 
-- -- -- My include directories are there for reference but you must use your own directory path
 
-- -- -- Locate the folder where you previously downloaded all the SDL's and include every include folder
 
-- -- On Library Directories (click on the text box, arrow, edit, new line, ...).
 
-- -- --  My library directories are there for reference but you must use your own directory path
 
-- -- -- Locate the folder where you previously downloaded all the SDL's and include every lib folder and choose x64

Hit Apply

In each of the library folders (lib/x64) there is a .dll file that MUST be saved to the same folder that the executable directory is in.

-- Should be similar to this 

<img width="195" height="213" alt="image" src="https://github.com/user-attachments/assets/f37fd50b-0617-4b60-b7d9-43d0e714eaf9" />

You are now able to run the code in Visual Studio 2022

