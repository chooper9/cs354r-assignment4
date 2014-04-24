#include "Game.h"

Game::Game(Ogre::SceneManager* mSceneMgr) {
	currentLevel= LV_NEPTUNE;

	debugging = false;
	paused = false;
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
		sceneSpace->setupScene(currentLevel);
		sceneSpace->addCamera(mainCam);
		currentScene = sceneSpace;
		break;
	case SCENE_PLANET:
		scenePlanet->setupScene(currentLevel);
		scenePlanet->addCamera(mainCam);
		currentScene = scenePlanet;
		CEGUI::WindowManager::getSingleton().getWindow("Pluto/PlanetRoot")->setVisible(true);
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
		CEGUI::WindowManager::getSingleton().getWindow("Pluto/PlanetRoot")->setVisible(false);
		break;
	default: break;
	}
	if (mainCam->getParentSceneNode() != NULL)
		mainCam->getParentSceneNode()->detachObject(mainCam);
	currentScene = NULL;
	currentGameScene = SCENE_NONE;
	debugging = false;
}

//-------------------------------------------------------------------------------------

void Game::runNextFrame(const Ogre::FrameEvent& evt) {
	if(currentGameScene != SCENE_NONE) currentScene->runNextFrame(evt);
	enum GameLevel lv;
	switch(currentGameScene) {
	case SCENE_SPACE:
		switch(sceneSpace->getResult(&lv)){
		case PLUTO_HIT_PLANET:
			currentLevel = lv;
			sceneSpace->hideScene();
			enterScene(SCENE_PLANET);
			break;
		case PLUTO_HIT_OBSTACLE: break;
		default: break;
		}
		break;
	case SCENE_PLANET: 
		switch(scenePlanet->getResult()){
		case PLUTO_WIN:
			if (debugging) return;
			exitScene();
			sceneSpace->finishLevel(currentLevel);
			sceneSpace->showScene();
			currentGameScene = SCENE_SPACE;
			currentScene = sceneSpace;
			break;
		case PLUTO_LOSE: break;
		default: break;
		}
		break;
	}
}

//-------------------------------------------------------------------------------------

void Game::handleKeyPressed(const OIS::KeyCode key) {
	switch (key) {
	case OIS::KC_B:
		switch (currentGameScene) {
		case SCENE_SPACE: 
			exitScene();
			enterScene(SCENE_PLANET);
			debugging = true;
			break;
		case SCENE_PLANET:
			exitScene(); break;
		default: 
			enterScene(SCENE_SPACE); break;
		}
		break;
	case OIS::KC_RETURN:
		CEGUI::WindowManager::getSingleton().getWindow("Pluto/PauseRoot")->setVisible(true);
		CEGUI::MouseCursor::getSingleton().show();
		break;
	default:break;
	}
	if(currentGameScene!= SCENE_NONE) currentScene->handleKeyPressed(key);
}

//-------------------------------------------------------------------------------------

void Game::handleKeyReleased(const OIS::KeyCode key) {
	if (paused) return;
	if(currentGameScene != SCENE_NONE) currentScene->handleKeyReleased(key);
}

//-------------------------------------------------------------------------------------

void Game::handleMouseMoved( int dx, int dy ) {
	if (paused) return;
	if(currentGameScene != SCENE_NONE) currentScene->handleMouseMoved(dx, dy);
}

//-------------------------------------------------------------------------------------

void Game::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	if (paused) return;
	if(currentGameScene != SCENE_NONE) currentScene->handleMousePressed(x, y, id);
}

//-------------------------------------------------------------------------------------

void Game::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	if (paused) return;
	if(currentGameScene != SCENE_NONE) currentScene->handleMouseReleased(x, y, id);
}
