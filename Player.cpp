#include "Player.h"
#include <iostream>

Player::Player(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, SceneTerrain* terrainOn, const Ogre::Vector3& pos) :  
	graphicsEngine(mSceneMgr),
	positionNode(0),
	visible(true),
	terrain(terrainOn),
	isPerformingShurikenAOE(false),
	physicsEngine(bulletEngine),
	hpbar(NULL),
	attackEffectChecked(true)
{
	shurikens.clear();
	int height = isPluto ? HEIGHT_PLUTO : HEIGHT_NINJA;
	playerState.defaultHP = isPluto ? HP_PLUTO : HP_NINJA;
	positionNode = parentNode->createChildSceneNode();
	isAI = !isPluto;
	playerEnt = mSceneMgr->createEntity("ninja.mesh" );
	playerEnt->setMaterialName("PlutoNinjaColors");
	playerEnt->setCastShadows(true);
	Ogre::SubEntity* playerSub = playerEnt->getSubEntity(0);
	playerSub->setCustomParameter(1, Ogre::Vector4(0.051f, 0.353f, 0.145f, 1.0f));
	//playerSub->setCustomParameter(1, Ogre::Vector4((209.0/255.0), (139.0/255.0), (27.0/255.0), 1.0f));

        playerEnt->getAnimationState("Idle1")->setLoop(true);
        playerEnt->getAnimationState("Walk")->setLoop(true);
        playerEnt->getAnimationState("Block")->setLoop(false);
        playerEnt->getAnimationState("Attack3")->setLoop(false);
        playerEnt->getAnimationState("SideKick")->setLoop(false);
        playerEnt->getAnimationState("Jump")->setLoop(false);
        playerEnt->getAnimationState("Backflip")->setLoop(false);
        playerEnt->getAnimationState("Spin")->setLoop(false);
        playerEnt->getAnimationState("Death1")->setLoop(false);

	if(isPluto) {
		//playerEnt->setMaterialName("Examples/Ninja/Blue");
		playerSub->setCustomParameter(1, Ogre::Vector4(0.047f, 0.094f, 0.302f, 1.0f));
		CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
		wmgr->getWindow("Pluto/PlanetRoot/HPBar")->setWidth(CEGUI::UDim(0.22, 0));
		wmgr->getWindow("Pluto/PlanetRoot/Blade")->setVisible(true);
		wmgr->getWindow("Pluto/PlanetRoot/Shuriken")->setVisible(false);
		wmgr->getWindow("Pluto/PlanetRoot/ShurikenCount")->setVisible(false);
	}

	Ogre::SceneNode* characterNode = positionNode->createChildSceneNode();
	characterNode->attachObject(playerEnt);
	Ogre::Real ratio = height/195.649;
	characterNode->scale(ratio, ratio, ratio);
	
	orient = positionNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

	physicsObject.setToBox(
		btVector3(10,height*0.8,5),
		100,
		btQuaternion(0, 0, 0, 1),
		btVector3(0, 0, 0)
	);
	physicsObject.toggleRigidBodyAndKinematic(); // change to Kinematic

	physicsObject.setRestitution(0.001); 
	physicsObject.getRigidBody()->setAngularFactor(0);
	physicsObject.setFriction(1);  

	physicsEngine->addObject(&physicsObject);
	resetState();
	positionNode->setPosition(pos.x, terrain->getHeightAtWorldPosition(pos), pos.z);
	setTransform(positionNode->getPosition(), positionNode->getOrientation());
}

//-------------------------------------------------------------------------------------

Player::~Player(void) {
	for (std::vector<Shuriken*>::iterator it = shurikens.begin(); it != shurikens.end(); it++)
		delete (*it);
	shurikens.clear();
	
	CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
	if (!isAI) {
		wmgr->getWindow("Pluto/PlanetRoot/Blade")->setVisible(false);
		wmgr->getWindow("Pluto/PlanetRoot/Shuriken")->setVisible(false);
		wmgr->getWindow("Pluto/PlanetRoot/ShurikenCount")->setVisible(false);
	}
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
	playerState.allowShuriken = false;
	playerState.step = STEP_NINJA;
	playerState.weapon = WEAPON_BLADE;
	playerState.numShuriken = 10000000;
	playerEnt->getSubEntity(1)->setVisible(true);
	playerState.degreeYaw = 0;
	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
	attackEffectChecked = true;
}

void Player::runNextFrame(const Ogre::FrameEvent& evt, Player* pluto, std::vector<Player*>& enemies) {
	std::vector<Shuriken*>::iterator it = shurikens.begin();
	while (it != shurikens.end()) {
		Shuriken* shuriken = (*it);
		if (!shuriken->isActive()) {			
			it++;
			continue;
		}
		Ogre::Vector3 shurikenPos = shuriken->getSceneNode()->getPosition();
		if (shurikenPos.y <= terrain->getHeightAtWorldPosition(shurikenPos)) {
			shuriken->deactivate();
			it++;
			continue;
		}
		shuriken->updateGraphicsScene();
		if (shuriken->collidesWith(pluto->getPhysicsObject()) == HIT_NINJA) {
			delete shuriken;
			it = shurikens.erase(it);
			pluto->hitBy(ATTACK_SHURIKEN);
			continue;
		}
		bool hit = false;
		for (std::vector<Player*>::iterator p = enemies.begin(); p != enemies.end(); p++) {
			if (shuriken->collidesWith((*p)->getPhysicsObject()) == HIT_NINJA) {
				delete shuriken;
				it = shurikens.erase(it);
				(*p)->hitBy(ATTACK_SHURIKEN);
				hit = true;
				break;
			}
		}
		if(!hit) it++;
	}

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
	case THROW_SHURIKEN:
		animation = playerEnt->getAnimationState("Spin");
		animation->addTime(evt.timeSinceLastFrame*1.4);
		if (animation->getTimePosition() > 0.73) {
			if (!attackEffectChecked && playerState.numShuriken > 0) {
				Ogre::Vector3 shurikenPos = positionNode->getPosition() + orient*20;
				shurikenPos.y += 50;
				Shuriken* s = new Shuriken(graphicsEngine, positionNode->getParentSceneNode(), physicsEngine, shurikenPos);
				s->getPhysicsObject().setLinearVelocity(btVector3(orient.x*150, 10, orient.z*150));
				shurikens.push_back(s);
				playerState.numShuriken--;
				sprintf(shuriken_buf, "x%d", playerState.numShuriken);
				CEGUI::WindowManager::getSingleton().getWindow("Pluto/PlanetRoot/ShurikenCount")->setText(shuriken_buf);
				if (!isPerformingShurikenAOE) attackEffectChecked = true;
			}
			if (animation->getTimePosition() == animation->getLength()) {
				playerState.action = IDLE;
				animation->setTimePosition(0);
				animation->setEnabled(false);
				attackEffectChecked = true;
			}
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
		animation = playerEnt->getAnimationState("SideKick");
		animation->addTime(evt.timeSinceLastFrame);
		if (animation->getTimePosition() == animation->getLength()) {
			playerState.action = IDLE;
			animation->setTimePosition(0);
			animation->setEnabled(false);
		}
		return; // don't proceed to walk while kicking
	case KICKED:
		animation = playerEnt->getAnimationState("Backflip");
		animation->addTime(evt.timeSinceLastFrame);
		if (animation->getTimePosition() == animation->getLength()) {
			playerState.action = IDLE;
			animation->setTimePosition(0);
			animation->setEnabled(false);
		}
		return; // don't proceed to walk while being kicked
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

	bool moved = direction.x != 0 || direction.z != 0;

	if(moved) {
		direction *= evt.timeSinceLastFrame;
		Ogre::Vector3 oldPos = positionNode->getPosition();
		positionNode->translate(direction, Ogre::Node::TS_LOCAL);
		// check collisions between ninjas
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
		Ogre::Vector3 newPos = positionNode->getPosition();
		newPos.y = terrain->getHeightAtWorldPosition(newPos);
		positionNode->setPosition(newPos);
	}
	

	if (playerState.action == IDLE){
		if (moved) {
			animation = playerEnt->getAnimationState("Walk");
			animation->addTime(evt.timeSinceLastFrame*(playerState.step == STEP_NINJA ? 1.5: 2.5));
		}else{
			animation = playerEnt->getAnimationState("Idle1");
			animation->addTime(evt.timeSinceLastFrame*0.5);
		}
	        animation->setEnabled(true);
	}

	setTransform(positionNode->getPosition(), positionNode->getOrientation());

}

//-------------------------------------------------------------------------------------

void Player::checkAttackEffect(Player* enemy) {
	if (!attackEffectChecked && playerEnt->getAnimationState("Attack3")->getTimePosition() > 0.3) {
		attackEffectChecked = true;
		if (enemy->playerEnt->getAnimationState("Block")->getTimePosition() < 0.24) {
			enemy->hitBy(ATTACK_BLADE);
		} else {
			//std::cout <<"  play sound - blocked" << std::endl;
			soundHandler->play_sound(sword_block);
		}
	} else if (!attackEffectChecked && playerEnt->getAnimationState("SideKick")->getTimePosition() > 0.3) {
		enemy->hitBy(ATTACK_KICK);
		enemy->kicked();
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
		if (enemy->playerState.action == ATTACK && playerState.action != ATTACK) {
			block();
			playerState.movingForward = false;
			playerState.movingBackward = false;
			return;
		} else {
			stopBlock();
			if (yaw < 10 && yaw > -10) {
				if (Ogre::Math::UnitRandom() < 0.9)
					attack();
				else
					kick();
			}
			checkAttackEffect(enemy);
		}
	} else {
		stopBlock();
		if(dist < 200 && playerState.allowShuriken) {
			if (Ogre::Math::UnitRandom() < 0.3)
				throwShuriken();
		}
	}

	if (yaw > 0) playerState.degreeYaw = 1;
	else if (yaw < 0) playerState.degreeYaw = -1;
	
	playerState.movingForward = (dist > 60 && dist < 500) || dist > 2000;
	playerState.movingBackward = dist < 20;
}

//-------------------------------------------------------------------------------------

void Player::setTransform(const Ogre::Vector3& pos, const Ogre::Quaternion& q) {
	orient = q * Ogre::Vector3::NEGATIVE_UNIT_Z;
	positionNode->setPosition(pos);
	positionNode->setOrientation(q);
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
	physicsObject.setWorldTransform(trans);
	if(isAI && hpbar)
	  hpbar->setPosition(pos.x, pos.y+80, pos.z);
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
	case OIS::KC_Q: 
		switchWeapon();
		break;
	case OIS::KC_H: 
		if (playerState.weapon == WEAPON_SHURIKEN) shurikenAOE();
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
	switch(id) {
	case OIS::MB_Left:
		switch(playerState.weapon) {
		case WEAPON_BLADE: attack(); break;
		case WEAPON_SHURIKEN: throwShuriken(); break;
		}
		break;
	case OIS::MB_Right:
		if (playerState.weapon != WEAPON_SHURIKEN)
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
