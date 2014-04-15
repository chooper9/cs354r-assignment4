#include "Shuriken.h"
#include <iostream>

Shuriken::Shuriken(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, const Ogre::Vector3& pos) :  
	graphicsEngine(mSceneMgr),
	positionNode(0),
	physicsEngine(bulletEngine),
	colliding(false),
	shooting(false)
{
	entShuriken = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	entShuriken->setCastShadows(true);
	entShuriken->setMaterialName("Examples/TennisBall");

	positionNode = parentNode->createChildSceneNode(pos);
	Ogre::SceneNode* tempNode = positionNode->createChildSceneNode();
	tempNode->attachObject(entShuriken);
	Ogre::Real ratio = Shuriken_RADIUS/100;
	tempNode->scale(ratio, ratio, ratio);
	
	physicsObject.setToSphere(
		Shuriken_RADIUS, 
		2, 
		btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(),
		btVector3(pos.x,pos.y,pos.z) 
	);
	physicsObject.setRestitution(0.99);
	physicsObject.setLinearVelocity(btVector3(0,0,0));
	physicsObject.setFriction(0.5);  
	physicsObject.setAngularVelocity(btVector3(0.2f, 0.5f, 0.2f));
	physicsEngine->addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

Shuriken::~Shuriken(void) {
	physicsEngine->removeObject(&physicsObject);
	destroySceneNodeHelper(positionNode);
	positionNode->removeAndDestroyAllChildren();
	graphicsEngine->destroySceneNode(positionNode);
	std::cout << "========= Debug: Shuriken Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void Shuriken::updateGraphicsScene(void) {
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	btQuaternion orientation = trans.getRotation();
	btVector3 origin = trans.getOrigin();
	positionNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
	positionNode->setPosition(Ogre::Vector3(origin.getX(), origin.getY(), origin.getZ()));
}

//-------------------------------------------------------------------------------------


ShurikenCollisionEvent Shuriken::collidesWith(PhysicsObject& obj) {
	ShurikenContactResultCallback callback;
	ShurikenCollisionEvent result = HIT_NOTHING;
        btDiscreteDynamicsWorld* world = physicsEngine->getPhysicsWorld();
	btRigidBody* me = physicsObject.getRigidBody();

	world->contactPairTest(me, obj.getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_NINJA;
	}
	return result;
}