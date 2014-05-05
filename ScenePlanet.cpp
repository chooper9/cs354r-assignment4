#include "ScenePlanet.h"

ScenePlanet::ScenePlanet(Ogre::SceneManager* mSceneMgr) : Scene::Scene(mSceneMgr) {
	cameraMode = CAM_THIRD_PERSON;
	enemies.clear();
	pluto = NULL;
	weather = NULL;
	terrain = OGRE_NEW SceneTerrain("terrain.png", "NeptuneTerrainTexture", graphicsEngine);
	terrain->hideTerrain();
	currentLevel = LV_NEPTUNE;
	std::cout << "========= Debug: ScenePlanet Created =========" << std::endl;
}

//-------------------------------------------------------------------------------------

ScenePlanet::~ScenePlanet(void) {
	destroyScene();
	OGRE_DELETE terrain;
	std::cout << "========= Debug: ScenePlanet Deleted =========" << std::endl;
}

bool ScenePlanet::setupScene(enum GameLevel level) {
	if (!Scene::setupScene(level)) return false;
	soundHandler->start_ambient(ambient_fire);
	int numEnemies = 0;
	switch(level) {
	case LV_NEPTUNE:
		numEnemies = 10;
		weather = graphicsEngine->createParticleSystem("Heavy Snow", "Examples/Snow");
		if (currentLevel != level)
			terrain->setMaterial("NeptuneTerrainTexture", 10);
		break;
	case LV_URANUS:
		weather = graphicsEngine->createParticleSystem("Heavy Rain", "Examples/Rain");
		numEnemies = 20;
		if (currentLevel != level)
			terrain->setMaterial("UranusTerrainTexture", 60);
		break;
	case LV_SATURN:
		// graphicsEngine->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(0.9, 0.9, 0.9), 0.0, 50, 500);
		weather = graphicsEngine->createParticleSystem("fog", "Examples/MyFog");
		numEnemies = 30;
		if (currentLevel != level)
			terrain->setMaterial("SaturnTerrainTexture", 20);
		break;
	case LV_JUPITER:
		numEnemies = 40;
		if (currentLevel != level)
			terrain->setMaterial("JupiterTerrainTexture", 30);
		break;
	case LV_MARS:
		numEnemies = 50;
		weather = graphicsEngine->createParticleSystem("Heavy Rain", "Examples/Rain");
		weather->setMaterialName("Examples/Droplet_Red");
		if (currentLevel != level)
			terrain->setMaterial("MarsTerrainTexture", 600);
		break;
	case LV_EARTH:
		numEnemies = 60;
		if (currentLevel != level)
			terrain->setMaterial("EarthTerrainTexture", 800);
		break;
	case LV_SUN:
		numEnemies = 200;
		if (currentLevel != level)
			terrain->setMaterial("PlutoTerrainTennisCourtTexture", 1);
		break;
	case LV_BOSS:
		numEnemies = 1;
		if (currentLevel != level)
			terrain->setMaterial("PlutoTerrainBossTexture", 800);
		break;
	}
	terrain->showTerrain();
	currentLevel = level;


	pluto = new Player(graphicsEngine, sceneRootNode, physicsEngine, true, terrain);
	if(weather != NULL)
		pluto->getSceneNode()->attachObject(weather);
	enemyHPset = graphicsEngine->createBillboardSet("EnemyHPSet");
	enemyHPset->setPoolSize(level*10);
	enemyHPset->setMaterialName("Pluto/EnemyHP");
	enemyHPset->setCastShadows(false);
	enemyHPset->setDefaultDimensions(8,2);
	enemyHPset->setBounds(Ogre::AxisAlignedBox(-1000,-1000,-1000,1000,1000,1000), 1000);

	Ogre::SceneNode* hpBBSnode = sceneRootNode->createChildSceneNode();
	hpBBSnode->attachObject(enemyHPset);
	enemies.clear();


	for(int i = 0; i < numEnemies; i++) {
		enemies.push_back(new Player(
			graphicsEngine, sceneRootNode, physicsEngine, false, terrain, Ogre::Vector3(i*40 - level*200, 0, -100)
		));
		enemies[i]->setBillboard(enemyHPset->createBillboard(Ogre::Vector3(i*40 - level*200, 80, -100)));
	}
	if (level == LV_BOSS) {
		PlayerState* ps = enemies[0]->getPlayerState();
		ps->defaultHP = HP_BOSS;
		ps->hp = HP_BOSS;
		ps->allowShuriken = true;
	}
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::destroyScene(void) {
	detachCamera(camera);
	if(weather != NULL && pluto) {
		pluto->getSceneNode()->detachObject(weather);
		graphicsEngine->destroyParticleSystem(weather);
	}
	weather = NULL;
	if (pluto) delete pluto;
	pluto = NULL;
	for (std::vector<Player*>::iterator it = enemies.begin(); it != enemies.end(); it++)
		delete (*it);
	enemies.clear();
	if (graphicsEngine->hasBillboardSet("EnemyHPSet"))
		graphicsEngine->destroyBillboardSet (enemyHPset);
	terrain->hideTerrain();
	if (!Scene::destroyScene()) return false;
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::addCamera(Ogre::Camera* cam, enum CameraMode camMode) {
	if(!Scene::addCamera(cam)) return false;
	Ogre::SceneNode* camNode = pluto->getSceneNode()->createChildSceneNode(
		camMode == CAM_THIRD_PERSON ? 
			Ogre::Vector3(0, 100, 70) : Ogre::Vector3(0, HEIGHT_NINJA*0.8, -5)
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
	case OIS::KC_P:
		terrain->mTerrainGroup->unloadTerrain(0,0);
		break;
	case OIS::KC_O:
		terrain->mTerrainGroup->loadTerrain(0,0, true);
		break;
	case OIS::KC_L:
		terrain->setMaterial("NeptuneTerrainTexture");
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
	if (!isSceneSetup) return;
	pluto->handleMouseMoved(dx, dy);
	if (camera && camera->getParentSceneNode()) {
		camera->getParentSceneNode()->pitch(Ogre::Degree(-dy*0.01), Ogre::Node::TS_LOCAL);
	}
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	if (!isSceneSetup) return;
	pluto->handleMousePressed(x, y, id);
}

//-------------------------------------------------------------------------------------

void ScenePlanet::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	if (!isSceneSetup) return;
	pluto->handleMouseReleased(x, y , id);
}
