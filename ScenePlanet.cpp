#include "ScenePlanet.h"

ScenePlanet::ScenePlanet(Ogre::SceneManager* mSceneMgr) : Scene::Scene(mSceneMgr) {
	cameraMode = CAM_THIRD_PERSON;
	enemies.clear();
	pluto = NULL;
	weather = NULL;
	weatherNode = mSceneMgr->createSceneNode("WeatherNode");
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
	int numEnemies = 0;
	Ogre::Vector3 enemy_color = Ogre::Vector3(0.051f, 0.353f, 0.145f);
	switch(level) {
	case LV_NEPTUNE:
		numEnemies = 10;
		weather = graphicsEngine->createParticleSystem("Heavy Snow", "Examples/Snow");
		enemy_color = Ogre::Vector3(0.860f, 0.883f, 0.933f);
		if (currentLevel != level)
			terrain->setMaterial("NeptuneTerrainTexture", 10);
		break;
	case LV_URANUS:
		weather = graphicsEngine->createParticleSystem("Heavy Rain", "Examples/Rain");
		numEnemies = 20;
		enemy_color = Ogre::Vector3(0.275f, 0.839f, 0.800f);
		if (currentLevel != level)
			terrain->setMaterial("UranusTerrainTexture", 60);
		break;
	case LV_SATURN:
		weather = graphicsEngine->createParticleSystem("fog", "Examples/MyFog");
		numEnemies = 30;
		enemy_color = Ogre::Vector3(0.819f, 0.545f, 0.106f);
		if (currentLevel != level)
			terrain->setMaterial("SaturnTerrainTexture", 20);
		break;
	case LV_JUPITER:
		numEnemies = 40;
		enemy_color = Ogre::Vector3(0.431f, 0.271f, 0.043f);
		weather = graphicsEngine->createParticleSystem("Firewall", "Pluto/FlameGeysers");
		if (currentLevel != level)
			terrain->setMaterial("JupiterTerrainTexture", 30);
		break;
	case LV_MARS:
		numEnemies = 50;
		weather = graphicsEngine->createParticleSystem("Heavy Rain", "Examples/Rain");
		weather->setMaterialName("Examples/Droplet_Red");
		enemy_color = Ogre::Vector3(0.612f, 0.043f, 0.043f);
		if (currentLevel != level)
			terrain->setMaterial("MarsTerrainTexture", 600);
		break;
	case LV_EARTH:
		numEnemies = 60;
		if (currentLevel != level)
			terrain->setMaterial("EarthTerrainTexture", 800);
		break;
	case LV_SUN:
		numEnemies = 100;
		CEGUI::WindowManager::getSingleton().getWindow("Pluto/EndGameRoot")->setVisible(false);
		if (currentLevel != level)
			terrain->setMaterial("PlutoTerrainTennisCourtTexture", 1);
		break;
	case LV_BOSS:
		soundHandler->start_ambient(bossbattle);
		numEnemies = 1;
		enemy_color = Ogre::Vector3(0.129f, 0.137f, 0.180f);
		if (currentLevel != level)
			terrain->setMaterial("PlutoTerrainBossTexture", 800);
		break;
	}
	terrain->showTerrain();
	currentLevel = level;


	pluto = new Player(graphicsEngine, sceneRootNode, physicsEngine, true, terrain);
	if (level == LV_SUN) {
		pluto->setThrowTennis(true);
	}
	if(weather != NULL) {
		weatherNode->attachObject(weather);
		if(weather->getName() == "Firewall")
			graphicsEngine->getRootSceneNode()->addChild(weatherNode);
		else
			pluto->getSceneNode()->addChild(weatherNode);
	}
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
		if (i < 5) {
			enemies.push_back(new Player(
				graphicsEngine, sceneRootNode, physicsEngine, false, terrain, Ogre::Vector3(i*40 - level*200, 0, -100)
			));
		}
		else if (i < 15) {
			enemies.push_back(new Player(
				graphicsEngine, sceneRootNode, physicsEngine, false, terrain, Ogre::Vector3(i*40 - level*200, 0, -500)
			));
		}
		else if (i < 45) {
			enemies.push_back(new Player(
				graphicsEngine, sceneRootNode, physicsEngine, false, terrain, Ogre::Vector3(i*40 - level*200, 0, -1000)
			));
		}
		else if (i < 100) {
			enemies.push_back(new Player(
				graphicsEngine, sceneRootNode, physicsEngine, false, terrain, Ogre::Vector3(i*40 - level*200, 0, -1500)
			));
		}
		else {
			enemies.push_back(new Player(
				graphicsEngine, sceneRootNode, physicsEngine, false, terrain, Ogre::Vector3(i*40 - level*200, 0, -2000)
			));
		}
		enemies[i]->setBillboard(enemyHPset->createBillboard(Ogre::Vector3(i*40 - level*200, 80, -100)));
		enemies[i]->setColor(enemy_color.x, enemy_color.y, enemy_color.z);
		if (level == LV_SUN)
			enemies[i]->setRandColor();
	}
	if(level != LV_SUN)
		pluto->getPlayerState()->numShuriken = 300;
	if (level == LV_BOSS) {
		PlayerState* ps = enemies[0]->getPlayerState();
		enemies[0]->setAsBoss();
		CEGUI::WindowManager::getSingleton().getWindow("Pluto/BossQuotes")->setVisible(true);
		ps->defaultHP = HP_BOSS;
		ps->hp = HP_BOSS;
		ps->allowShuriken = true;
	}
}

//-------------------------------------------------------------------------------------

bool ScenePlanet::destroyScene(void) {
	detachCamera(camera);
	soundHandler->play_explosion();
	if(weather != NULL && pluto) {
		if(weather->getName() == "Firewall")
			graphicsEngine->getRootSceneNode()->removeChild("WeatherNode");
		else
			pluto->getSceneNode()->removeChild("WeatherNode");
		weatherNode->detachObject(weather);
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
	int enemiesLeft = 0;
	bool run = false;
	for (std::vector<Player*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		Player* enemy = *it;
		if (!enemy->isDead() /*&& !run*/){
			pluto->reactTo(enemy);
			enemy->reactTo(pluto);
			run = true;
			++enemiesLeft;
		}
		enemy->runNextFrame(evt, pluto, enemies);
	}
	sprintf(num_enemies_buf, "%d", enemiesLeft);
	std::string enemy_string = "Enemies Remaining: ";
	enemy_string += num_enemies_buf;
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/PlanetRoot/EnemyCount")->setText(enemy_string);
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
	if (pluto->isDead()) 
	{
		soundHandler->play_sound(pluto_dead);
		return PLUTO_LOSE;
	}
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
