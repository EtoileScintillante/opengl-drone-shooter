# Drone Shooter
Drone Shooter is a simple FPS game created with OpenGL.  
The goal is to shoot as many drones as you can before they kill you.
## Controls
- W - go forwards
- S - go backwards
- A - go left
- D - go right
- Space - take shot
- Mouse - look around
## About
Drone Shooter was created using just OpenGL, no game engine. It's purely done for fun and learning purposes.   
Some of the code is based on code from the [learnopengl tutorial by Joey de Vries](https://learnopengl.com/).  
The game objects (models, sounds, font and textures) were all downloaded for free from the internet.  
I used sites like [free3d](https://free3d.com/), [soundjay](https://www.soundjay.com/gun-sound-effect.html) and [freebpr](https://freepbr.com/).  
To implement collision detection between the player’s bullet and enemy and between the enemy’s laser beam and player [this](https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection) site was a great resource.   
For adding audio to the game I used the [miniaudio](https://github.com/mackron/miniaudio) library.   
For text rendering I used the [freetype](http://freetype.org/) library.
### Player
There is no actual player character, it’s just a gun. The direction of the bullet is the same as the front vector of the player, which makes aiming easy. The player’s bounding box looks as follows:

<img src="https://github.com/EtoileScintillante/opengl-fps/blob/master/doc/player_bbox/front_side.png" width="200" height="210" alt="bounding box player" />

There is also an ‘aim assist’ option, if this is turned on there will be a red cross rendered in the center of the screen.
#### Aim assist on
<img src="https://github.com/EtoileScintillante/opengl-fps/blob/master/doc/aim_assist_on.png" width="400" height="250" alt="aim assist on" />

#### Aim assist off
<img src="https://github.com/EtoileScintillante/opengl-fps/blob/master/doc/aim_assist_off.png" width="400" height="250" alt="aim assist off" />

### Enemy
The enemy is a drone that can shoot laser beams. Its aim is not perfect; some randomness is added when calculating the direction of the laser beam, otherwise every shot would lead to a decrease in the player’s health.   
The bounding box of the enemy does not cover the whole drone, as can be seen in the image below. It may look weird but it’s not a problem at all. The bounding box is still big enough and it’s not hard to hit the drone, even when standing far away. 

<img src="https://github.com/EtoileScintillante/opengl-fps/blob/master/doc/enemy_bbox/front_side.png" width="400" height="250" alt="bounding box enemy" />

## Evolution of the game
When I first started this project the idea was to create a Minecraft scenery, with Minecraft trees and grass for the terrain and Minecraft mobs as enemies.
 I ended up with something completely different.   
 During the development I regularly uploaded videos to YouTube to keep track of the progress I made.

- [12 Sept. 2022](https://youtu.be/qpYmfjfTjCA) Minecraft scenery
- [23 Sept. 2022](https://youtu.be/ef7LN32q7J0) migration to more realistic scenery
- [3 Oct. 2022](https://youtu.be/Qg2YRp42_rg) added sound effects
- [17 Oct. 2022](https://youtu.be/Spv8I2WCzuU) improved enemy movement and added text on screen
- [22 Oct. 2022](https://youtu.be/Ox03BBIdRAc) enemies can kill player

