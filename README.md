cs354r-assignment4
==================

Pluto game

FEATURES
        Martial arts battles with ninjas
        Above-head display (Enemy unit name/health)
        Enemy unit AI
        Two scenes - one for space travel, the other for ninja fights
        Implement terrain
        Particle effects (Distant stars, comets, space debris, planet weather)
        CEGUI for storytelling and interface (pause menu)
        Try using text files to generate levels
        Ranged attack physics
        Minimal story
        
        
Code Architecture
        Pluto - Inherits BaseApplication, handles render queue
  Game - Main game logic, dispatch user input to different scenes; switch between scenes.
  Scene - abstract class for scenes: handles particle system, handles user input 
          passed from Game class; handle AI; pass game state back to Game class for scene change.
  SceneSpace - implement Scene; Setup the scene in space
  ScenePlanet - implement Scene; Setup the battlefield on planets
  GUI - handles GUI; player health bar, in game text,  etc...
        Sound - static methods that play sound
        Terrain - setup terrain; used by ScenePlanet
  Player - setup enemy ninjas as well as Pluto on the planet, implement above-head display 
  Planet - setup characters in space (basically spheres)
Division of labor
        Jiawei - Game, part of Scene classes, input handling.
        Cameron - GUI, Player above-head display, text display
        Anthony - Terrain
        Kyle - create scenes and research meshes, particle systems.
