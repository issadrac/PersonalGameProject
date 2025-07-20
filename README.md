# MyGame
MyGame is a 2D platformer built with the SDL3 library. It is heavily inspired by classic Mario-style games. This project serves as a learning exercise to understand the implementation of core game development concepts, such as user input, hitboxes, images, and working with sound. The game is very scalable, supporting multiple levels and hostiles per level.

Game Features

Multiple Levels: Includes a functional first level and a proof-of-concept second level to demonstrate multiple levels working.

Moving Hostiles: Enemies that move back-and-forth or in one direction.

Moving Platforms: Platforms that move up and down or left and right.

Debugging Tools:

Green Button: Highlights all the current hitboxes during gameplay.

Ctrl+Click on Green Button: Separates platforms by color (Red, Green, Blue) and displays a number at the top of the screen that displays the real world x position for that column. 

Red Button: Slows down the game to help debug specific issues in real-time.

I created MyGame to learn how to implement essential game mechanics such as user input, collision detection, level loading, and sound. Through this project, I also focused on building debugging tools that help with visualizing hitboxes and detecting issues in gameplay. The debugging tools I developed have been extremely useful for optimizing the game and ensuring it runs as expected.

The main menu is first shown which consists of two blue buttons and a red button. The first level is entered by pressing the blue button to the left. That level is the one that should be played. The second level is entered by pressing the blue button to the right but that is only there to show proof of multiple levels working rather than attempting to make an engaging level. The red button in the main menu exits the program when clicked.

## How to run

### 1. Install SDL Libraries

Download SDL3 https://libsdl.org/release/?C=M;O=D

--- Choose this one: SDL3-devel-3.2.10-VC.zip

Download SDL3_image https://www.libsdl.org/projects/SDL_image/release/?C=M;O=D

--- Choose this one: SDL3_image-devel-3.2.4-VC.zip

Download SDL3_ttf https://www.libsdl.org/projects/SDL_ttf/release/

--- Choose this one: SDL3_ttf-devel-3.1.0-VC.zip

Extract all into the same folder for later convenience 

### 2. Clone everything into Visual Studio 2022 Community Version.
<img width="364" height="145" alt="image" src="https://github.com/user-attachments/assets/13693a61-20ba-4257-9d10-75579ca80ac1" />

### 3. Right click on MyGame

<img width="463" height="813" alt="image" src="https://github.com/user-attachments/assets/504d7088-fbc7-45bf-9a04-3b01a989b048" />

### 4. Click Properties and On Configuration Properties

#### Click on VC++ Directories

##### On Include Directories (click on the text box, arrow, edit, new line, ...)
 
-- -- -- My include directories are there for reference but you must use your own directory path
 
-- -- -- Locate the folder where you previously downloaded all the SDL's and include every include folder
 
##### On Library Directories (click on the text box, arrow, edit, new line, ...).
 
-- -- --  My library directories are there for reference but you must use your own directory path
 
-- -- -- Locate the folder where you previously downloaded all the SDL's and include every lib folder and choose x64

### 5. Hit Apply

### 6. In each of the library folders (lib/x64) there is a .dll file that MUST be saved to the same folder that the executable directory is in.

-- Should be similar to this 

<img width="195" height="213" alt="image" src="https://github.com/user-attachments/assets/f37fd50b-0617-4b60-b7d9-43d0e714eaf9" />

You are now able to run the code in Visual Studio 2022

## Movement Controls

Jump: Up-Arrow/Space

Left: Left-Arrow

Right: Right-Arrow
