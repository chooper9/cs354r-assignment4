#include "Planet.h"
#include <iostream>

Planet::Planet(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, int type, const Ogre::Vector3& pos) :
	graphicsEngine(mSceneMgr),
	positionNode(0),
	physicsEngine(bulletEngine)
{
	positionNode = parentNode->createChildSceneNode(pos);
	isAI = !isPluto;
	planetEnt = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	planetEnt->setCastShadows(true);
	planetNode = positionNode->createChildSceneNode();
	planetNode->attachObject(planetEnt);
	Ogre::Real ratio;
	destroyed = false;
	if (isAI) {
		if (type == 0) {
			planetState.size = SIZE_PLANET1;
			planetEnt->setMaterialName("Examples/Neptune");
			ratio = SIZE_PLANET1/100;
			explosion = graphicsEngine->createParticleSystem("neptuneExplosion", "Examples/Explosion");
			explosionSmoke = graphicsEngine->createParticleSystem("neptuneExplosionSmoke", "Examples/ExplodeSmoke");
		}
		else if (type == 1) {
			planetState.size = SIZE_PLANET2;
			planetEnt->setMaterialName("Examples/Uranus");
			ratio = SIZE_PLANET1/100;
			explosion = graphicsEngine->createParticleSystem("uranusExplosion", "Examples/Explosion");
			explosionSmoke = graphicsEngine->createParticleSystem("uranusExplosionSmoke", "Examples/ExplodeSmoke");
		}
		else if (type == 2) {
			planetState.size = SIZE_PLANET3;
			planetEnt->setMaterialName("Examples/Saturn");
			ratio = SIZE_PLANET3/100;
			explosion = graphicsEngine->createParticleSystem("saturnExplosion", "Examples/Explosion");
			explosionSmoke = graphicsEngine->createParticleSystem("saturnExplosionSmoke", "Examples/ExplodeSmoke");
		}
		else if (type == 3) {
			planetState.size = SIZE_PLANET4;
			planetEnt->setMaterialName("Examples/Jupiter");
			ratio = SIZE_PLANET4/100;
			explosion = graphicsEngine->createParticleSystem("jupiterExplosion", "Examples/Explosion");
			explosionSmoke = graphicsEngine->createParticleSystem("jupiterExplosionSmoke", "Examples/ExplodeSmoke");
		}
		else if (type == 4) {
			planetState.size = SIZE_PLANET5;
			planetEnt->setMaterialName("Examples/Mars");
			ratio = SIZE_PLANET5/100;
			explosion = graphicsEngine->createParticleSystem("marsExplosion", "Examples/Explosion");
			explosionSmoke = graphicsEngine->createParticleSystem("marsExplosionSmoke", "Examples/ExplodeSmoke");
		}
		else if (type == 5) {
			planetState.size = SIZE_PLANET6;
			planetEnt->setMaterialName("Examples/Earth");
			ratio = SIZE_PLANET6/100;
			explosion = graphicsEngine->createParticleSystem("earthExplosion", "Examples/Explosion");
			explosionSmoke = graphicsEngine->createParticleSystem("earthExplosionSmoke", "Examples/ExplodeSmoke");
		}
		else if (type == 6) {
			planetState.size = SIZE_SUN;
			planetEnt->setMaterialName("Examples/Sun");
			ratio = SIZE_SUN/100;
			explosion = NULL;
		}
	} else {
		planetState.size = SIZE_PLUTO;
		planetEnt->setMaterialName("Examples/Pluto");
		explosion = NULL;
		ratio = SIZE_PLUTO/100;
	}
	planetNode->scale(ratio, ratio, ratio);
	physicsObject.setToSphere(
		ratio * 100, 
		2,
		btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(),
		btVector3(pos.x, pos.y, pos.z)
	);
	physicsObject.setRestitution(0.99);
	physicsObject.setLinearVelocity(btVector3(0,0,0));
	physicsObject.setFriction(0.5);  
	physicsObject.setAngularVelocity(btVector3(0.2f, 0.5f, 0.2f));
	physicsEngine->addObject(&physicsObject);
	resetState();
}

Planet::~Planet(void) {
	physicsEngine->removeObject(&physicsObject);
	destroySceneNodeHelper(positionNode);
	positionNode->removeAndDestroyAllChildren();
	graphicsEngine->destroySceneNode(positionNode);
	std::cout << "========= Debug: Planet Deleted =========" << std::endl;
}

void Planet::resetState(void) {
	planetState.hp = 100;
	planetState.step = FLY_PLANET;
	planetState.movingLeft = false;
	planetState.movingRight = false;
	planetState.movingForward = false;
	planetState.movingBackward = false;
	planetState.movingUpward = false;
	planetState.movingDownward = false;
	planetState.degreeYaw = 0;
	planetState.degreePitch = 0;
}


void Planet::runNextFrame(const Ogre::FrameEvent& evt, Planet* pluto, std::vector<Planet*>& enemies) {
	
	Ogre::Vector3 direction(0, 0, 0);
	if (planetState.movingLeft) {
		direction.x -= planetState.step;	
	}
	if (planetState.movingRight)
		direction.x += planetState.step;
	if (planetState.movingForward)
		direction.z -= planetState.step;
	if (planetState.movingBackward)
		direction.z += planetState.step;
	if (planetState.movingUpward)
		direction.y += planetState.step;
	if (planetState.movingDownward)
		direction.y -= planetState.step;

	bool moved = direction.x != 0 || direction.z != 0 || direction.y != 0;
	
	planetNode->yaw(Ogre::Degree(0.05), Ogre::Node::TS_LOCAL);

	//positionNode->pitch(Ogre::Degree(planetState.degreePitch), Ogre::Node::TS_LOCAL);
	positionNode->yaw(Ogre::Degree(planetState.degreeYaw), Ogre::Node::TS_LOCAL);
	planetState.degreeYaw = 0;
	planetState.degreePitch = 0;



	if(moved) {
		direction *= evt.timeSinceLastFrame;
		Ogre::Vector3 oldPos = positionNode->getPosition();
		positionNode->translate(direction, Ogre::Node::TS_LOCAL);
		
		
		// check collisions between Planetoids
		/*
		Ogre::Vector3 pushedPosition(0, 0, 0);
		for (std::vector<Player*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
			Player* enemy = *it;
			if (isAI && this == enemy) continue;
			if (this != enemy && !enemy->isDead() && isCollidingWith(enemy)){
				positionNode->translate(direction*(-0.6), Ogre::Node::TS_LOCAL);
				if (isCollidingWith(enemy, pushedPosition))
					positionNode->setPosition(pushedPosition);
				break;
			}
		}
		if (isAI && !pluto->isDead() && isCollidingWith(pluto)) {
			positionNode->translate(direction*(-0.6), Ogre::Node::TS_LOCAL);
			if (isCollidingWith(pluto, pushedPosition))
				positionNode->setPosition(pushedPosition);
		}
		*/
	}
	setTransform(positionNode->getPosition(), positionNode->getOrientation());
}

void Planet::setTransform(const Ogre::Vector3& pos, const Ogre::Quaternion& q) {
	positionNode->setPosition(pos);
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
	physicsObject.setWorldTransform(trans);
}

void Planet::handleKeyPressed(const OIS::KeyCode key) {
	switch(key) {
	case OIS::KC_W: 
		planetState.movingForward = true; break;
	case OIS::KC_A: 
		planetState.movingLeft = true; break;
	case OIS::KC_S: 
		planetState.movingBackward = true; break;
	case OIS::KC_D: 
		planetState.movingRight = true; break;
	case OIS::KC_Q: 
		planetState.movingUpward = true; break;
	case OIS::KC_E: 
		planetState.movingDownward = true; break;
	case OIS::KC_LCONTROL: 
		planetState.step = FLY_PLANET_FAST; break;
	}
}

void Planet::handleKeyReleased(const OIS::KeyCode key) {
	switch(key) {
	case OIS::KC_W: 
		planetState.movingForward = false; break;
	case OIS::KC_A: 
		planetState.movingLeft = false; break;
	case OIS::KC_S: 
		planetState.movingBackward = false; break;
	case OIS::KC_D: 
		planetState.movingRight = false; break;
	case OIS::KC_Q: 
		planetState.movingUpward = false; break;
	case OIS::KC_E: 
		planetState.movingDownward = false; break;
	case OIS::KC_LCONTROL:
		planetState.step = FLY_PLANET; 
		break;
	}
}

//-------------------------------------------------------------------------------------

void Planet::handleMouseMoved( int dx, int dy ) {
	planetState.degreeYaw = -dx/10.0;
	planetState.degreePitch = -dy/10.0;
	
}

void Planet::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	switch(id) {
	case OIS::MB_Left:
		
		break;
	case OIS::MB_Right:
		
		break;
	}
}

//-------------------------------------------------------------------------------------

void Planet::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	switch(id) {
	case OIS::MB_Right:
		
		break;
	}
}
