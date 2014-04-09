#include "Player.h"
#include <iostream>

Player::Player(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, const Ogre::Vector3& pos) :  
	graphicsEngine(mSceneMgr),
	positionNode(0),
	visible(true),
	physicsEngine(bulletEngine)
{
	resetState();
	int height = HEIGHT_NINJA;

	positionNode = parentNode->createChildSceneNode(pos);

	playerEnt = mSceneMgr->createEntity( "playerEnt", "ninja.mesh" );
	playerEnt->setCastShadows(true);

        playerEnt->getAnimationState("Idle1")->setLoop(true);
        playerEnt->getAnimationState("Walk")->setLoop(true);
        playerEnt->getAnimationState("Block")->setLoop(false);
        playerEnt->getAnimationState("Attack3")->setLoop(false);
        playerEnt->getAnimationState("Kick")->setLoop(false);
        playerEnt->getAnimationState("Jump")->setLoop(false);


	Ogre::SceneNode* characterNode = positionNode->createChildSceneNode();
	characterNode->attachObject(playerEnt);
	Ogre::Real ratio = height/195.649;
	characterNode->scale(ratio, ratio, ratio);
	
	physicsObject.setToBox(
		btVector3(10,height,5),
		100,
		btQuaternion(0, 0, 0, 1),
		btVector3(pos.x, pos.y, pos.z)
	);
	physicsObject.toggleRigidBodyAndKinematic(); // change to Kinematic

	physicsObject.setRestitution(0.001); 
	physicsObject.getRigidBody()->setAngularFactor(0);
	physicsObject.setFriction(1);  

	physicsEngine->addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

Player::~Player(void) {
	physicsEngine->removeObject(&physicsObject);
	positionNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity(playerEnt);
	std::cout << "========= Debug: Player Deleted =========" << std::endl;
}

void Player::toggleVisible(void) {
	if (visible) {
		physicsEngine->removeObject(&physicsObject);
		positionNode->detachObject(playerEnt);
		visible = false;
	} else {
		physicsEngine->addObject(&physicsObject);
		positionNode->attachObject(playerEnt);
		visible = true;
	}
}

void Player::resetState(void) {
	playerState.action = IDLE;
	playerState.hp = HP_PLUTO;
	playerState.step = STEP_NINJA;
	playerState.degreeYaw = 0;
	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
}

void Player::nextAction(const Ogre::FrameEvent& evt) {
	if (playerState.action != IDLE){
	        playerEnt->getAnimationState("Walk")->setEnabled(false);
	        playerEnt->getAnimationState("Idle1")->setEnabled(false);
	}

	Ogre::AnimationState* animation;
	switch(playerState.action) {
	case ATTACK:
		animation = playerEnt->getAnimationState("Attack3");
		animation->setEnabled(true);
		animation->addTime(evt.timeSinceLastFrame);
		if (animation->getTimePosition() == animation->getLength()) {
			playerState.action = IDLE;
			animation->setTimePosition(0);
			animation->setEnabled(false);
		}
		break;
	case BLOCK:
		playerEnt->getAnimationState("Block")->addTime(evt.timeSinceLastFrame);
		break;
	case STOP_BLOCK:
		animation = playerEnt->getAnimationState("Block");
		animation->addTime(-evt.timeSinceLastFrame);
		if (animation->getTimePosition() <= 0) {
			playerState.action = IDLE;
			animation->setEnabled(false);
		}
		break;
	case JUMP:
		animation = playerEnt->getAnimationState("Jump");
		animation->addTime(evt.timeSinceLastFrame);
		if (animation->getTimePosition() == animation->getLength()) {
			playerState.action = IDLE;
			animation->setTimePosition(0);
			animation->setEnabled(false);
		}
		break;
	case KICK:
		animation = playerEnt->getAnimationState("Kick");
		animation->addTime(evt.timeSinceLastFrame);
		if (animation->getTimePosition() == animation->getLength()) {
			playerState.action = IDLE;
			animation->setTimePosition(0);
			animation->setEnabled(false);
		}
		break;
	default: break;
	}

	if (playerState.action == KICK) return;


	positionNode->yaw(Ogre::Degree(playerState.degreeYaw), Ogre::Node::TS_LOCAL);
	playerState.degreeYaw = 0;
	
	Ogre::Vector3 direction(0, 0, 0);
	if (playerState.movingLeft)
		direction.x -= playerState.step;
	if (playerState.movingRight)
		direction.x += playerState.step;
	if (playerState.movingForward)
		direction.z -= playerState.step;
	if (playerState.movingBackward)
		direction.z += playerState.step;
	direction *= evt.timeSinceLastFrame;

	positionNode->translate(direction, Ogre::Node::TS_LOCAL);

	if (playerState.action == IDLE){
		if (direction.x != 0 || direction.z != 0)
			animation = playerEnt->getAnimationState("Walk");
		else
			animation = playerEnt->getAnimationState("Idle1");
	        animation->setEnabled(true);
		animation->addTime(evt.timeSinceLastFrame);
	}

	Ogre::Quaternion q = positionNode->getOrientation();
	Ogre::Vector3 p = positionNode->getPosition();
	btTransform playerTrans;
	physicsObject.getWorldTransform(playerTrans);
	playerTrans.setOrigin(btVector3(p.x, p.y, p.z));
	playerTrans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
	physicsObject.setWorldTransform(playerTrans);

}

//-------------------------------------------------------------------------------------

void Player::setPosition(const Ogre::Vector3& pos) {
	positionNode->setPosition(pos);
	positionNode->resetOrientation();
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(btQuaternion(0, 0, 0, 1));
	physicsObject.setWorldTransform(trans);
}
//-------------------------------------------------------------------------------------

void Player::handleKeyPressed(const OIS::KeyCode key) {
	switch(key) {
	case OIS::KC_W: 
		playerState.movingForward = true; break;
	case OIS::KC_A: 
		playerState.movingLeft = true; break;
	case OIS::KC_S: 
		playerState.movingBackward = true; break;
	case OIS::KC_D: 
		playerState.movingRight = true; break;
	case OIS::KC_SPACE: 
		if (playerState.action == IDLE) {
			playerState.action = JUMP;
			playerEnt->getAnimationState("Jump")->setEnabled(true);
		}
		break;
	case OIS::KC_E: 
		if (playerState.action == IDLE) {
			playerState.action = KICK;
			playerEnt->getAnimationState("Kick")->setEnabled(true);
		}
		break;
	}
}

//-------------------------------------------------------------------------------------

void Player::handleKeyReleased(const OIS::KeyCode key) {
	switch(key) {
	case OIS::KC_W: 
		playerState.movingForward = false; break;
	case OIS::KC_A: 
		playerState.movingLeft = false; break;
	case OIS::KC_S: 
		playerState.movingBackward = false; break;
	case OIS::KC_D: 
		playerState.movingRight = false; break;
	}
}

//-------------------------------------------------------------------------------------

void Player::handleMouseMoved( int dx, int dy ) {
	playerState.degreeYaw = -dx/10.0;
}

//-------------------------------------------------------------------------------------

void Player::handleMousePressed( int x, int y, OIS::MouseButtonID id ) {
	if (playerState.action != IDLE) return;
	switch(id) {
	case OIS::MB_Left:
		playerState.action = ATTACK;
		playerEnt->getAnimationState("Attack3")->setEnabled(true);
		break;
	case OIS::MB_Right:
		playerState.action = BLOCK;
		playerEnt->getAnimationState("Block")->setEnabled(true);
		break;
	}
}

//-------------------------------------------------------------------------------------

void Player::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	switch(id) {
	case OIS::MB_Right:
		if (playerState.action == BLOCK){
			playerState.action = STOP_BLOCK;
		}
		break;
	}
}
