# Assignment 3 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".
If you are answering a criteria in a different section, please describe that like: "4a. This criteria is met by the implementation described in 2b."

## Section 1: Design Pitch. Describe a possible setting and what kind of graphics technology would be necessary to implement this. These components do not have to be implemented in this project. Suggestion: 200 words should be more than enough for this.

Greetings and welcome to G3D designed by yours truly Eddy Zhang. Ever wanted to pick up a complex platformer without the need to follow a story or get invested into the characters? Then this is the game for you. With seamless controls and a colourful, in your face art style G3D is very easy to get hooked on and will make you feel like you're in the game (©EA). This is a rhythm style platformer with little to no worldbuilding, just a sit back and play type. The objective is to get the little cube from the start to the end of each level's variety of obstacles using a variety of controls, including jumps, gravity flipping and even flying in some sections.

On the technological side, HDR lighting, magic mirrors and kernel effects will be required in order to immerse the player in the fast and exciting nature of the game. Most of the graphics in the game will be focused on lighting effects, I would suggest ray tracing, particle effects and temporal post processing for motion blur, but these will not be implemented in this assignment.
## Section 2: Advanced Lighting

### 2a: Blinn-Phong Lighting. Include code references to the implementation of lighting in your project.
I implemented Blinn-Phong Lighting in the default fragment shader on lines 59 and 87. This was used for all objects that used the default lighting shaders, which excludes the mirrors as I wanted those to be seen clearly.

### 2b: High Dynamic Range. Show use of an intermediate frame buffer to store light values > 1.0 and how that is converted to Low Dynamic Range. Include code references.

I implemented high dynamic range lighting through a screen quad. I used a screen quad to save the framebuffer of the rendered scene pre-hdr and applied the hdr shaders hdr.frag and hdr.vert to it. The code in the function post_process_hdr in renderer.cpp, used on line 84 of main.cpp initalises the shaders and the shaders mentioned before do the hdr work. I used an exposure tone mapping algorithm found on the website https://learnopengl.com/Advanced-Lighting/HDR and set the exposure to 0.3 because it looked good.

## Section 3: Reflections

### 3a: Environment Mapped Reflections. Show at least one object can reflect a cube map. Cube map can be a preprepared skybox and the object doesn't need to reflect other objects in the scene. Include code references to implementation.

After opening the application, you can see the skybox cubemap in the background. The code for making the cubemap is in cubemap.cc, the code for drawing it is in the function draw_sky on line 79 of renderer.cpp and the shaders used are skybox.frag and skybox.vert. 

### 3b: Realtime Cube Map. Describe how the cube map is created during the frame render and then used for a reflective object. Include code references to the generation of the cube map (3a should already describe the reflection process).

I tried to do this but could not get it to work. The code is in render_reflective in renderer.cpp.

### 3c: In-World Camera/Magic Mirror. Describe how you have placed a camera in a scene that renders to a texture. Show where both of these objects are and how they function to show a different viewpoint of the scene. Include references to code.

On the left and right side of the scene, you can see two volumes which are both in-world cameras pointing from the perspective of the right-most torus. I put a camera in the scene using the make_camera function in camera.cpp to read to a framebuffer and used the framebuffer texture as the volume texture. The code for this is in camera.cpp, the framebuffer code is in framebuffer.cpp, the binding to a framebuffer is in main.cpp at lines 69 to 77. The draw_mirror function in renderer.cpp draws the mirror using the shaders mirror.vert and mirror.frag.

### 3d: Planar Reflections. Describe the system used to generate a planar reflection, including any techniques for perspective and/or near plane correction. Include references to code.

I did not complete part 3d. (If you would like your 3d to be marked, you will need to replace this line)

## Section 4: Post Processing

### 4a: Simple Colour Change. Describe how this project uses a framebuffer and what change is made to the colour data, including references to where this is implemented in code.

2b complete.

### 4b: Use of Kernels. Describe the kernel sampling technique in use, including code references.

When opening the code, you can see that there is an 'edge detection' kernel in place and when the cube goes through the middle torus, a 'sharpen' kernel is used, both found from https://learnopengl.com/Advanced-OpenGL/Framebuffers. I used a framebuffer to sample the post-hdr render and apply it to a quad which was then used to apply the kernels. The code for this is in post_process on line 333 of renderer.cpp and the shaders are  filter for edge detection and filterkern for the sharpen.

### 4c: Multiple Intermediate Framebuffers. Show use of more than one framebuffer for post processing (like chaining two post processing effects in a row). Include references to code.

I used multiple intermediate framebuffers in order to use hdr and kernels by chaining these two effects. See hdr and kernel sections for code references.

### 4d: Multiple Render Targets. Describe how this project uses multiple frame buffers to write information from the initial fragment shader pass and how it uses them in post processing. Include references to fragment shader output code as well as post processing code.

I did not complete part 4d. (If you would like your 4d to be marked, you will need to replace this line)

### 4e: Temporal Post Processing. Show storage of framebuffers from previous frame(s) and how they are being used in post processing. Include references to where this is happening in code.

I did not complete part 4e. (If you would like your 4e to be marked, you will need to replace this line)

## Section 5: Show Me What You Got. Implement any techniques that are not part of previous criteria for assignments in this course. This is open, so must be documented clearly with code references to show what has been created to add to the visual effects of the scene or objects in it. 

I did not complete part 5. (If you would like your 5 to be marked, you will need to replace this line)

## Section 6: Subjective Mark

### How have you fulfilled this requirement?

I got inspiration for this from the game geometry dash, a fast-paced rhythm obstacle game and I have always wondered what a 3d version of it would be like.

### What is your vision for the scene you are showing?

I also wanted to improve on my assignment 2 by making the scene more colourful and with more effects. I used the mirrors to give an idea that the cube is repeating its path.

### Are you referencing anything in this scene? Is this a quote or homage to any other art? Please describe it.

Geometry dash the game.

### Please give instructions for how a viewer/player should interact with this scene to see all the content you have created.

Fly around, look at the mirrors and watch the kernel change happen when the cube hits the middle torus.

## Section 7 (optional): Code Style Feedback

### There are no marks assigned to code style in this assignment. However, if you would like to receive some feedback on your coding style, please point out a section of your code (up to 100 lines) and your tutor will give you some written feedback on the structure and readability of that code.

Write your answer here (lines numbers or function names that you'd like reviewed)

## Section 8 (optional): Use of External Libraries

N/A

### What does the library do?

Write your answer here...

### Why did you decide to use it?

Write your answer here...

### How does this integrate with the assignment project build system? Please include any instructions and requirements if a marker needs to integrate an external library.

Write your answer here...
