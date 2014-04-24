#ifndef __Scene_h_
#define __Scene_h_

#include "GameResource.h"
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

class Scene
{
protected:
	bool isSceneSetup;
	Ogre::Camera* camera;
	Ogre::SceneManager* graphicsEngine;
	PhysicsEngine* physicsEngine;
	Ogre::SceneNode* sceneRootNode;
public:
	Scene(Ogre::SceneManager* mSceneMgr);
	~Scene(void);
	void showScene(void) { 
		if (!isSceneSetup) return; addCamera(camera); sceneRootNode->setVisible(true);
	}
	void hideScene(void) { 
		if (!isSceneSetup) return; sceneRootNode->setVisible(false);
	}
	virtual bool setupScene(enum GameLevel level);
	virtual bool destroyScene(void);
	virtual bool addCamera(Ogre::Camera* cam, enum CameraMode camMode=CAM_THIRD_PERSON);
	virtual bool runNextFrame(const Ogre::FrameEvent& evt);
	virtual void handleKeyPressed(const OIS::KeyCode key) = 0;
	virtual void handleKeyReleased(const OIS::KeyCode key) = 0;
	virtual void handleMouseMoved( int dx, int dy ) = 0;
	virtual void handleMousePressed( int x, int y, OIS::MouseButtonID id ) = 0;
	virtual void handleMouseReleased( int x, int y, OIS::MouseButtonID id ) = 0;
};

#endif // #ifndef __Scene_h_
