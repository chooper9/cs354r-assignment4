#include "SceneSpace.h"

SceneSpace::SceneSpace(Ogre::SceneManager* mSceneMgr) : Scene(mSceneMgr) {
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 50000);
	enemies.clear();
	pluto = NULL;
        std::cout << "========= Debug: SceneSpace Created =========" << std::endl;
}

//-------------------------------------------------------------------------------------

SceneSpace::~SceneSpace(void) {
	destroyScene();
        std::cout << "========= Debug: SceneSpace Deleted =========" << std::endl;

}

//-------------------------------------------------------------------------------------

bool SceneSpace::setupScene(int level) {
	if (!Scene::setupScene(level)) return false;
	
	enemies.clear();
	pluto = new Planet(graphicsEngine, sceneRootNode, physicsEngine, true, Ogre::Vector3(0,100, 10000));
	enemies.push_back(new Planet(graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(0, 0, 5000)));
	enemies.push_back(new Planet(graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(0, -1000, -5000)));
	enemies.push_back(new Planet(graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(0, -2000, -12000)));
	enemies.push_back(new Planet(graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(0, 1000, -18000)));
	enemies.push_back(new Planet(graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(0, 2000, -22000)));
}

//-------------------------------------------------------------------------------------

bool SceneSpace::destroyScene(void) {
	detachCamera(camera);
	if (pluto) delete pluto;
	pluto = NULL;
	for (std::vector<Planet*>::iterator it = enemies.begin(); it != enemies.end(); it++)
		delete (*it);
	enemies.clear();
	if (!Scene::destroyScene()) return false;
}

//-------------------------------------------------------------------------------------

bool SceneSpace::addCamera(Ogre::Camera* cam, enum CameraMode camMode) {
	if(!Scene::addCamera(cam)) return false;
	Ogre::SceneNode* camNode = pluto->getSceneNode()->createChildSceneNode(
		Ogre::Vector3(0, 400, 800)
	);
	camNode->pitch(Ogre::Degree(-20));
        camNode->attachObject(cam);
	return true;
}

//-------------------------------------------------------------------------------------

void SceneSpace::runAI(const Ogre::FrameEvent& evt) {
	for (std::vector<Planet*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		Planet* enemy = *it;
		if (enemy->isDestroyed()) continue;
		enemy->runNextFrame(evt, pluto, enemies);
	}
}


//-------------------------------------------------------------------------------------

bool SceneSpace::runNextFrame(const Ogre::FrameEvent& evt) {
	if(!Scene::runNextFrame(evt)) return false;
	pluto->runNextFrame(evt, pluto, enemies);
	runAI(evt);
	return true;
}

//-------------------------------------------------------------------------------------

SceneSpaceResult SceneSpace::getResult(int* level) {
	int lv = 0;
	for (std::vector<Planet*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		if (pluto->isCollidingWith(*it)) {
			*level = lv;
			return PLUTO_HIT_PLANET;
		}
		lv++;
	}
	return PLUTO_TRAVELLING;
	// return PLUTO_HIT_OBSTACLE 
}

//-------------------------------------------------------------------------------------

void SceneSpace::handleKeyPressed(const OIS::KeyCode key) {
	if (!isSceneSetup) return;
	pluto->handleKeyPressed(key);

	switch(key) {
	case OIS::KC_W: break;
	}
}

//-------------------------------------------------------------------------------------

void SceneSpace::handleKeyReleased(const OIS::KeyCode key) {
	if (isSceneSetup) pluto->handleKeyReleased(key);
}

//-------------------------------------------------------------------------------------

void SceneSpace::handleMouseMoved( int dx, int dy ) {
	if (isSceneSetup) pluto->handleMouseMoved(dx, dy);
}

//-------------------------------------------------------------------------------------

void SceneSpace::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	if (isSceneSetup) pluto->handleMousePressed(x, y, id);
}

//-------------------------------------------------------------------------------------

void SceneSpace::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	if (isSceneSetup) pluto->handleMouseReleased(x, y , id);
}
