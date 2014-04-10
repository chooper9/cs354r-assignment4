#include "Player.h"
#include <iostream>

Player::Player(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, const Ogre::Vector3& pos) :  
	graphicsEngine(mSceneMgr),
	positionNode(0),
	visible(true),
	physicsEngine(bulletEngine),
	attackEffectChecked(true)
{
	int height = isPluto ? HEIGHT_PLUTO : HEIGHT_NINJA;
	playerState.defaultHP = isPluto ? HP_PLUTO : HP_NINJA;
	positionNode = parentNode->createChildSceneNode(pos);
	isAI = !isPluto;
	playerEnt = mSceneMgr->createEntity("ninja.mesh" );
	playerEnt->setCastShadows(true);

        playerEnt->getAnimationState("Idle1")->setLoop(true);
        playerEnt->getAnimationState("Walk")->setLoop(true);
        playerEnt->getAnimationState("Block")->setLoop(false);
        playerEnt->getAnimationState("Attack3")->setLoop(false);
        playerEnt->getAnimationState("Kick")->setLoop(false);
        playerEnt->getAnimationState("Jump")->setLoop(false);
        playerEnt->getAnimationState("Death1")->setLoop(false);


	Ogre::SceneNode* characterNode = positionNode->createChildSceneNode();
	characterNode->attachObject(playerEnt);
	Ogre::Real ratio = height/195.649;
	characterNode->scale(ratio, ratio, ratio);
	
	orient = positionNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

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
	resetState();
}

//-------------------------------------------------------------------------------------

Player::~Player(void) {
	physicsEngine->removeObject(&physicsObject);

	destroySceneNodeHelper(positionNode);
	positionNode->removeAndDestroyAllChildren();
	graphicsEngine->destroySceneNode(positionNode);
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
	playerState.hp = playerState.defaultHP;
	playerState.step = STEP_NINJA;
	playerState.degreeYaw = 0;
	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
	attackEffectChecked = true;
}

void Player::runNextFrame(const Ogre::FrameEvent& evt) {
	if (isDead()) {
		die();
	}

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
			attackEffectChecked = true;
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
		return; // don't proceed to walk while kicking
	case DIE:
		animation = playerEnt->getAnimationState("Death1");
		animation->addTime(evt.timeSinceLastFrame);
		return; // don't proceed to walk while dying
	default: break;
	}

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
		if (direction.x != 0 || direction.z != 0) {
			animation = playerEnt->getAnimationState("Walk");
			animation->addTime(evt.timeSinceLastFrame*(playerState.step == STEP_NINJA ? 1.5: 2.5));
		}else{
			animation = playerEnt->getAnimationState("Idle1");
			animation->addTime(evt.timeSinceLastFrame*0.5);
		}
	        animation->setEnabled(true);
	}

	Ogre::Quaternion q = positionNode->getOrientation();
	
	orient = q * Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 p = positionNode->getPosition();
	btTransform playerTrans;
	physicsObject.getWorldTransform(playerTrans);
	playerTrans.setOrigin(btVector3(p.x, p.y, p.z));
	playerTrans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
	physicsObject.setWorldTransform(playerTrans);

}

//-------------------------------------------------------------------------------------

void Player::checkAttackEffect(Player* enemy) {
	if (!attackEffectChecked && 
		playerEnt->getAnimationState("Attack3")->getTimePosition() > 0.3 && 
		enemy->playerEnt->getAnimationState("Block")->getTimePosition() < 0.15
	) {
		enemy->hitBy(ATTACK_BLADE);
		attackEffectChecked = true;
	}else 
	if (!attackEffectChecked && playerEnt->getAnimationState("Kick")->getTimePosition() > 0.3) {
		enemy->hitBy(ATTACK_KICK);
		attackEffectChecked = true;
	}
}

//-------------------------------------------------------------------------------------

void Player::reactTo(Player* enemy) {
	if(playerState.action == DIE || enemy->playerState.action == DIE ) {
		playerState.movingForward = false;
		playerState.movingBackward = false;
		return;
	}
	Ogre::Vector3 dirToEnemy = enemy->positionNode->getPosition() - positionNode->getPosition();
	Ogre::Real dist = dirToEnemy.length();
	Ogre::Real yaw = orient.getRotationTo(dirToEnemy).getYaw().valueDegrees();


	if (!isAI) {
		if (dist < 80 && yaw < 10 && yaw > -10 && 
			(playerState.action == ATTACK || playerState.action == KICK))
			checkAttackEffect(enemy);
		return;
	}


	if (dist < 90) {
		if (isAI && enemy->playerState.action == ATTACK) {
			block();
			playerState.movingForward = false;
			playerState.movingBackward = false;
			return;
		} else {
			if (isAI) stopBlock();
			if (yaw < 10 && yaw > -10) {
				if (isAI) attack();
				checkAttackEffect(enemy);
			}
		}
	}
	if (yaw > 0) playerState.degreeYaw = 1;
	else if (yaw < 0) playerState.degreeYaw = -1;
	
	playerState.movingForward = dist > 60;
	playerState.movingBackward = dist < 60;


}

//-------------------------------------------------------------------------------------

void Player::setPosition(const Ogre::Vector3& pos) {
	positionNode->setPosition(pos);
	positionNode->resetOrientation();
	orient = positionNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
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
	case OIS::KC_LCONTROL: 
		playerState.step = STEP_NINJA_RUN; break;
	case OIS::KC_SPACE: 
		jump();
		break;
	case OIS::KC_E: 
		kick();
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
	case OIS::KC_LCONTROL: 
		playerState.step = STEP_NINJA; break;
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
		attack();
		break;
	case OIS::MB_Right:
		block();
		break;
	}
}

//-------------------------------------------------------------------------------------

void Player::handleMouseReleased( int x, int y, OIS::MouseButtonID id ) {
	switch(id) {
	case OIS::MB_Right:
		stopBlock();
		break;
	}
}
