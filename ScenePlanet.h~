
#ifndef __ScenePlanet_h_
#define __ScenePlanet_h_

#include "GameResource.h"
#include "Scene.h"
#include "Player.h"
#include "Shuriken.h"
class ScenePlanet : public Scene
{
private:
	Player* pluto;
	std::vector<Player*> enemies;
	enum CameraMode cameraMode;
	void runAI(const Ogre::FrameEvent& evt);
public:
	ScenePlanet(Ogre::SceneManager* mSceneMgr);
	~ScenePlanet(void);
	ScenePlanetResult getResult(void);
	virtual bool setupScene(int level);
	virtual bool destroyScene(void);
	virtual bool addCamera(Ogre::Camera* cam, enum CameraMode camMode=CAM_THIRD_PERSON);
	virtual bool runNextFrame(const Ogre::FrameEvent& evt);
	virtual void handleKeyPressed(const OIS::KeyCode key);
	virtual void handleKeyReleased(const OIS::KeyCode key);
	virtual void handleMouseMoved( int dx, int dy );
	virtual void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	virtual void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
};

#endif // #ifndef __ScenePlanet_h_
