#include "Game.h"

Game::Game(Ogre::SceneManager* mSceneMgr) {
	graphicsEngine = mSceneMgr;
	mainCam = graphicsEngine->getCamera("MainCam");
	currentGameScene = SCENE_NONE;
	currentScene = NULL;
	scenePlanet = new ScenePlanet(mSceneMgr);
	sceneSpace = new SceneSpace(mSceneMgr);
	std::cout << "========= Debug: Game Created =========" << std::endl;
}

//-------------------------------------------------------------------------------------

Game::~Game(void) {
	delete scenePlanet;
	delete sceneSpace;
	std::cout << "========= Debug: Game Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void Game::enterScene(enum GameScene newGameScene) {
	switch (newGameScene) {
	case SCENE_SPACE: 
		sceneSpace->setupScene(0);
		sceneSpace->addCamera(mainCam);
		currentScene = sceneSpace;
		break;
	case SCENE_PLANET:
		scenePlanet->setupScene(1);
		scenePlanet->addCamera(mainCam);
		currentScene = scenePlanet;
		break;
	default: 
		currentScene = NULL;
		break;
	}
	currentGameScene = newGameScene;
}

//-------------------------------------------------------------------------------------

void Game::exitScene(void) {
	switch (currentGameScene) {
	case SCENE_SPACE: 
		sceneSpace->destroyScene();
		break;
	case SCENE_PLANET:
		scenePlanet->destroyScene();
		break;
	default: break;
	}
	if (mainCam->getParentSceneNode() != NULL)
		mainCam->getParentSceneNode()->detachObject(mainCam);
	currentScene = NULL;
	currentGameScene = SCENE_NONE;
}

//-------------------------------------------------------------------------------------

void Game::runNextFrame(const Ogre::FrameEvent& evt) {
	if(currentGameScene != SCENE_NONE) currentScene->runNextFrame(evt);
}

//-------------------------------------------------------------------------------------

void Game::handleKeyPressed(const OIS::KeyCode key) {
	switch (key) {
	case OIS::KC_B:
		switch (currentGameScene) {
		case SCENE_SPACE: 
			exitScene();
			enterScene(SCENE_PLANET);
			break;
		case SCENE_PLANET:
			exitScene(); break;
		default: 
			enterScene(SCENE_SPACE); break;
		}
		break;
	default:break;
	}
	if(currentGameScene!= SCENE_NONE) currentScene->handleKeyPressed(key);
}

//-------------------------------------------------------------------------------------

void Game::handleKeyReleased(const OIS::KeyCode key) {
	if(currentGameScene != SCENE_NONE) currentScene->handleKeyReleased(key);
}

//-------------------------------------------------------------------------------------

void Game::handleMouseMoved( int dx, int dy ) {
	if(currentGameScene != SCENE_NONE) currentScene->handleMouseMoved(dx, dy);
}

//-------------------------------------------------------------------------------------

void Game::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	if(currentGameScene != SCENE_NONE) currentScene->handleMousePressed(x, y, id);
}

//-------------------------------------------------------------------------------------

void Game::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	if(currentGameScene != SCENE_NONE) currentScene->handleMouseReleased(x, y, id);
}
