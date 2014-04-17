#include "SceneSpace.h"

SceneSpace::SceneSpace(Ogre::SceneManager* mSceneMgr) : Scene(mSceneMgr) {
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"space", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, LENGTH_SceneSpace, WIDTH_SceneSpace, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z
	);
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 50000);
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
	
	//Ogre::Entity* entSide = graphicsEngine->createEntity("space");
	//entSide->setMaterialName("Examples/KAMEN");
	//entSide->setCastShadows(false);
	//sceneRootNode->attachObject(entSide);
	pluto = new Planet(graphicsEngine, sceneRootNode, physicsEngine, true, Ogre::Vector3(0,100, 0));
	targetPlanet = new Planet(graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(0, 100, -20000));
}

//-------------------------------------------------------------------------------------

bool SceneSpace::destroyScene(void) {
	if (camera->getParentSceneNode() != NULL)
		camera->getParentSceneNode()->detachObject(camera);
	if (pluto) delete pluto;
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

bool SceneSpace::runNextFrame(const Ogre::FrameEvent& evt) {
	if(!Scene::runNextFrame(evt)) return false;
	pluto->runNextFrame(evt, pluto, enemies);
	return true;
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
