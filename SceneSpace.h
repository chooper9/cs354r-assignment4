#ifndef __SceneSpace_h_
#define __SceneSpace_h_

#include "GameResource.h"
#include "Scene.h"
#include "Planet.h"


class SceneSpace : public Scene
{
private:
	Planet* pluto;
	std::vector<Planet*> enemies;
	Planet* sun;
	void runAI(const Ogre::FrameEvent& evt);
	bool sceneFinished;
public:
	SceneSpace(Ogre::SceneManager* mSceneMgr);
	~SceneSpace(void);
	void finishLevel(enum GameLevel level) {
		if(level == LV_BOSS) sceneFinished = true;
		else if(level == LV_SUN) {
			sun->setDestroyed(true);
			pluto->setTransform(Ogre::Vector3(0,100, 10000));
		}else enemies.at(level)->setDestroyed(true);
	}
	SceneSpaceResult getResult(enum GameLevel* level);
	virtual bool setupScene(enum GameLevel level);
	virtual bool destroyScene(void);
	virtual void hideScene(void) { 
		Scene::hideScene();
		if(pluto) pluto->stopMoving();
	}
	virtual bool addCamera(Ogre::Camera* cam, enum CameraMode camMode=CAM_THIRD_PERSON);
	virtual bool runNextFrame(const Ogre::FrameEvent& evt);
	virtual void handleKeyPressed(const OIS::KeyCode key);
	virtual void handleKeyReleased(const OIS::KeyCode key);
	virtual void handleMouseMoved( int dx, int dy );
	virtual void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	virtual void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
};

#endif // #ifndef __SceneSpace_h_
