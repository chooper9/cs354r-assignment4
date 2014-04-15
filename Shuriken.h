
#ifndef __Shuriken_h_
#define __Shuriken_h_

#include "GameResource.h"


class Shuriken {
private:
	ShurikenCollisionEvent collidesWithHelper(ShurikenCollisionEvent result);
protected:
	const static Ogre::Real Shuriken_RADIUS = 5.0f;
	Ogre::SceneManager* graphicsEngine;
	Ogre::SceneNode* positionNode;
	Ogre::Entity* entShuriken;
	PhysicsEngine* physicsEngine;
	PhysicsObject physicsObject;
	bool colliding;
	bool shooting;
public:
	Shuriken(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Shuriken(void);
	Ogre::SceneNode* getSceneNode(void) { return positionNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void updateGraphicsScene(void);
	ShurikenCollisionEvent collidesWith(PhysicsObject& obj/*not implemented*/);
};
#endif // #ifndef __Shuriken_h_