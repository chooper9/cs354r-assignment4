#ifndef __Player_h_
#define __Player_h_

#include "GameResource.h"

class Player {
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* playerEnt;
	Ogre::SceneNode* positionNode;

	bool visible;
	PhysicsObject physicsObject;
	PhysicsEngine* physicsEngine;
	PlayerState playerState;
public:
	Player(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Player(void);
	void toggleVisible(void);
	void resetState(void);
	void nextAction(const Ogre::FrameEvent& evt);
	void setPosition(const Ogre::Vector3& pos);
	Ogre::SceneNode* getSceneNode(void) { return positionNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void handleKeyPressed(const OIS::KeyCode key);
	void handleKeyReleased(const OIS::KeyCode key);
	void handleMouseMoved( int dx, int dy );
	void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
};
#endif // #ifndef __Player_h_
