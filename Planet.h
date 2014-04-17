#ifndef __Planet_h_
#define __Planet_h_

#include "GameResource.h"

class Planet {
private:
	bool isAI;
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* planetEnt;
	Ogre::SceneNode* positionNode;
	PhysicsObject physicsObject;
	PhysicsEngine* physicsEngine;
	PlanetState planetState;
public:
	Planet(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, const Ogre::Vector3& pos = Ogre::Vector3::ZERO);
	~Planet(void);
	void resetState(void);
	void runNextFrame(const Ogre::FrameEvent& evt, Planet* pluto, std::vector<Planet*>& enemies);
	void setTransform(const Ogre::Vector3& pos=Ogre::Vector3::ZERO, const Ogre::Quaternion& q=Ogre::Quaternion::IDENTITY);
	Ogre::SceneNode* getSceneNode(void) { return positionNode; }
	Ogre::Entity* getEntity(void) { return planetEnt; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void handleKeyPressed(const OIS::KeyCode key);
	void handleKeyReleased(const OIS::KeyCode key);
	void handleMouseMoved( int dx, int dy );
	void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
};
#endif // #ifndef __Planet_h_
