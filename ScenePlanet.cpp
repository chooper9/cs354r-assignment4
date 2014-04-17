#include "ScenePlanet.h"

ScenePlanet::ScenePlanet(Ogre::SceneManager* mSceneMgr) : Scene::Scene(mSceneMgr) {
	cameraMode = CAM_THIRD_PERSON;
	enemies.clear();
	pluto = NULL;
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

	PhysicsObject* ground = new PhysicsObject();
	ground->setToStaticPlane(btVector3(0,1,0), 0);
	physicsEngine->addObject(ground);

	pluto = new Player(graphicsEngine, sceneRootNode, physicsEngine, true);
	enemyHPset = graphicsEngine->createBillboardSet("EnemyHPSet");
	enemyHPset->setPoolSize(level*10);
	enemyHPset->setMaterialName("Pluto/EnemyHP");
	enemyHPset->setDefaultDimensions(8,2);
	enemyHPset->setBounds(Ogre::AxisAlignedBox(-1000,-1000,-1000,1000,1000,1000), 1000);
	Ogre::SceneNode* hpBBSnode = sceneRootNode->createChildSceneNode();
	hpBBSnode->attachObject(enemyHPset);
	enemies.clear();
	for(int i = 0; i < level*10; i++) {
		enemies.push_back(new Player(
			graphicsEngine, sceneRootNode, physicsEngine, false, Ogre::Vector3(i*40 - level*200, 0, -100)
		));
		enemies[i]->setBillboard(enemyHPset->createBillboard(Ogre::Vector3(i*40 - level*200, 80, -100)));
	}
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::destroyScene(void) {
	detachCamera(camera);
	if (pluto) delete pluto;
	pluto = NULL;
	for (std::vector<Player*>::iterator it = enemies.begin(); it != enemies.end(); it++)
		delete (*it);
	enemies.clear();
	if (graphicsEngine->hasBillboardSet("EnemyHPSet"))
		graphicsEngine->destroyBillboardSet (enemyHPset);
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

void ScenePlanet::runAI(const Ogre::FrameEvent& evt) {
	int i = 0;
	bool run = false;
	for (std::vector<Player*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		Player* enemy = *it;
		if (!enemy->isDead() /*&& !run*/){
			pluto->reactTo(enemy);
			enemy->reactTo(pluto);
			run = true;
		}
		enemy->runNextFrame(evt, pluto, enemies);
	}
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::runNextFrame(const Ogre::FrameEvent& evt) {
	if(!Scene::runNextFrame(evt)) return false;
	physicsEngine->stepSimulation(evt.timeSinceLastFrame*5);
	pluto->runNextFrame(evt, pluto, enemies);
	runAI(evt);
	return true;
}

//-------------------------------------------------------------------------------------

ScenePlanetResult ScenePlanet::getResult(void) {
	if (pluto->isDead()) return PLUTO_LOSE;
	bool botsAllDead = true;
	for (std::vector<Player*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		Player* enemy = *it;
		if (!enemy->isDead()){
			botsAllDead = false;
		}
	}
	if (botsAllDead)
		return PLUTO_WIN;
	else
		return PLUTO_FIGHTING;
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
	if (!isSceneSetup) return;
	pluto->handleKeyReleased(key);
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
