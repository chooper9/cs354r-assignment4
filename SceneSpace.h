#ifndef __SceneSpace_h_
#define __SceneSpace_h_

#include "GameResource.h"
#include "Scene.h"

class SceneSpace : public Scene
{
public:
	SceneSpace(Ogre::SceneManager* mSceneMgr);
	~SceneSpace(void);
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

#endif // #ifndef __SceneSpace_h_
