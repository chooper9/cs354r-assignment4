
#ifndef __Game_h_
#define __Game_h_

#include "GameResource.h"
#include "ScenePlanet.h"
#include "SceneSpace.h"


class Game
{
private:
	enum GameLevel currentLevel;
	bool debugging;
	bool gamePaused;

	Ogre::SceneManager* graphicsEngine;
	Ogre::SceneNode* titleNode;
	Ogre::ParticleSystem* titleParticles;
	Ogre::Camera* mainCam;
	enum GameScene currentGameScene;
	ScenePlanet* scenePlanet;
	SceneSpace* sceneSpace;
	Scene* currentScene;
public:
	Game(Ogre::SceneManager* mSceneMgr);
	~Game(void);
	void unPause() { gamePaused = false; }
	void enterScene(enum GameScene newGameScene);
	void exitScene(void);
	void runNextFrame(const Ogre::FrameEvent& evt);
	void handleKeyPressed(const OIS::KeyCode key);
	void handleKeyReleased(const OIS::KeyCode key);
	void handleMouseMoved( int dx, int dy );
	void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
protected:	
};

#endif // #ifndef __Game_h_
