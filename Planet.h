#ifndef __Planet_h_
#define __Planet_h_

#include "GameResource.h"

class Planet {
private:
	bool isAI;
	bool destroyed;
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* planetEnt;
	Ogre::SceneNode* positionNode;
	Ogre::SceneNode* planetNode;
	PhysicsObject physicsObject;
	PhysicsEngine* physicsEngine;
	PlanetState planetState;
public:
	Planet(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, const Ogre::Vector3& pos = Ogre::Vector3::ZERO);
	~Planet(void);
	void resetState(void);
	void stopMoving(void) {
		planetState.movingLeft = false;
		planetState.movingRight = false;
		planetState.movingForward = false;
		planetState.movingBackward = false;
		planetState.movingUpward = false;
		planetState.movingDownward = false;
	}
	void runNextFrame(const Ogre::FrameEvent& evt, Planet* pluto, std::vector<Planet*>& enemies);
	bool isDestroyed(void) { return destroyed; }
	void setDestroyed(bool d) { destroyed = d; positionNode->scale(0.1, 0.1, 0.1); }
	void setTransform(const Ogre::Vector3& pos=Ogre::Vector3::ZERO, const Ogre::Quaternion& q=Ogre::Quaternion::IDENTITY);
	Ogre::SceneNode* getSceneNode(void) { return positionNode; }
	Ogre::Entity* getEntity(void) { return planetEnt; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void handleKeyPressed(const OIS::KeyCode key);
	void handleKeyReleased(const OIS::KeyCode key);
	void handleMouseMoved( int dx, int dy );
	void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
	bool isCollidingWith(Planet* other) {
		if (other->destroyed) return false;
		bool collision = positionNode->getPosition().distance(other->positionNode->getPosition()) <= (planetState.size + other->planetState.size);
		if (collision) {	
			planetState.movingLeft = false;
			planetState.movingRight = false;
			planetState.movingForward = false;
			planetState.movingBackward = false;
		}
		return collision;
	}
};
#endif // #ifndef __Planet_h_
