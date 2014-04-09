#include "ScenePlanet.h"

ScenePlanet::ScenePlanet(Ogre::SceneManager* mSceneMgr) : Scene::Scene(mSceneMgr) {
	cameraMode = CAM_THIRD_PERSON;
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"planetSurface", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, LENGTH_ScenePlanet, WIDTH_ScenePlanet,1,1,true,1,1,1, Ogre::Vector3::UNIT_Z
	);
	std::cout << "========= Debug: ScenePlanet Created =========" << std::endl;
}

//-------------------------------------------------------------------------------------

ScenePlanet::~ScenePlanet(void) {
	destroyScene();
	std::cout << "========= Debug: ScenePlanet Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::setupScene(int level) {
	if (!Scene::setupScene(level)) return false;
	Ogre::Entity* entSide = graphicsEngine->createEntity("planetSurface");
	entSide->setMaterialName("Examples/KAMEN");
	entSide->setCastShadows(false);
	sceneRootNode->attachObject(entSide);

	pluto = new Player(graphicsEngine, sceneRootNode, physicsEngine);

}

//-------------------------------------------------------------------------------------

bool ScenePlanet::destroyScene(void) {
	if (camera->getParentSceneNode() != NULL)
		camera->getParentSceneNode()->detachObject(camera);
	if (pluto) delete pluto;
	if (!Scene::destroyScene()) return false;
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::addCamera(Ogre::Camera* cam, enum CameraMode camMode) {
	if(!Scene::addCamera(cam)) return false;
	Ogre::SceneNode* camNode = pluto->getSceneNode()->createChildSceneNode(
		camMode == CAM_THIRD_PERSON ? 
			Ogre::Vector3(0, 100, 100) : Ogre::Vector3(0, HEIGHT_NINJA*0.8, -5)
	);
	if (camMode == CAM_THIRD_PERSON)
		camNode->pitch(Ogre::Degree(-20));
        camNode->attachObject(cam);
	return true;
}

//-------------------------------------------------------------------------------------

void ScenePlanet::runNextFrame(const Ogre::FrameEvent& evt) {
	if (!isSceneSetup) return;
	pluto->nextAction(evt);
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleKeyPressed(const OIS::KeyCode key) {
	if (!isSceneSetup) return;
	pluto->handleKeyPressed(key);	

	switch(key) {
	case OIS::KC_R:
		if (cameraMode == CAM_FIRST_PERSON) {
			addCamera(camera, CAM_THIRD_PERSON);
			cameraMode = CAM_THIRD_PERSON;
		} else {
			addCamera(camera, CAM_FIRST_PERSON);
			cameraMode = CAM_FIRST_PERSON;
		}
		break;
	}
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleKeyReleased(const OIS::KeyCode key) {
	if (isSceneSetup) pluto->handleKeyReleased(key);
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleMouseMoved( int dx, int dy ) {
	if (isSceneSetup) pluto->handleMouseMoved(dx, dy);
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	if (isSceneSetup) pluto->handleMousePressed(x, y, id);
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	if (isSceneSetup) pluto->handleMouseReleased(x, y , id);
}
