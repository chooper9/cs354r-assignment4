#include "Scene.h"

Scene::Scene(Ogre::SceneManager* mSceneMgr) : 
	isSceneSetup(false),
	camera(NULL),
	graphicsEngine(mSceneMgr),
	physicsEngine(NULL),
	sceneRootNode(NULL)
{
}

//-------------------------------------------------------------------------------------

Scene::~Scene(void) {
	destroyScene();
}

//-------------------------------------------------------------------------------------

bool Scene::setupScene(enum GameLevel level) {
	if(isSceneSetup) return false;
	physicsEngine = new PhysicsEngine();
	sceneRootNode = graphicsEngine->getRootSceneNode()->createChildSceneNode();
	isSceneSetup = true;
	std::cout << "========= Debug: Scene Setup =========" << std::endl;
	return true;
}

//-------------------------------------------------------------------------------------

bool Scene::destroyScene(void) {
	if(!isSceneSetup) return false;
	delete physicsEngine;
	if (camera->getParentSceneNode() != NULL)
		camera->getParentSceneNode()->detachObject(camera);
	destroySceneNodeHelper(sceneRootNode);
	sceneRootNode->removeAndDestroyAllChildren();
	sceneRootNode->getCreator()->destroySceneNode(sceneRootNode);
	sceneRootNode = NULL;
	isSceneSetup = false;
	std::cout << "========= Debug: Scene Destroyed =========" << std::endl;
	return true;
}

//-------------------------------------------------------------------------------------

bool Scene::addCamera(Ogre::Camera* cam, enum CameraMode camMode) {
	if(!isSceneSetup) return false;
	detachCamera(cam);
	camera = cam;
	return true;
}

//-------------------------------------------------------------------------------------

bool Scene::runNextFrame(const Ogre::FrameEvent& evt) {
	return isSceneSetup;
}
