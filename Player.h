#ifndef __Player_h_
#define __Player_h_

#include "GameResource.h"
#include "Shuriken.h"
#include "SceneTerrain.h"
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>
#include <stdio.h>

class Player {
private:
	std::vector<Shuriken*> shurikens;
	bool attackEffectChecked;
	bool isPerformingShurikenAOE;
	Ogre::Vector3 orient;
	bool visible;
	bool isAI;
	SceneTerrain* terrain;
	void checkAttackEffect(Player* enemy);
	char shuriken_buf[10];
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* playerEnt;
	Ogre::SceneNode* positionNode;
	Ogre::Billboard* hpbar;
	PhysicsObject physicsObject;
	PhysicsEngine* physicsEngine;
	PlayerState playerState;
public:
	Player(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, SceneTerrain* terrainOn, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Player(void);
	void toggleVisible(void);
	void resetState(void);
	void runNextFrame(const Ogre::FrameEvent& evt, Player* pluto, std::vector<Player*>& enemies);
	void reactTo(Player* enemy);
	void setBillboard(Ogre::Billboard* bb) { hpbar = bb; }
	void setTransform(const Ogre::Vector3& pos=Ogre::Vector3::ZERO, const Ogre::Quaternion& q=Ogre::Quaternion::IDENTITY);
	Ogre::SceneNode* getSceneNode(void) { return positionNode; }
	Ogre::Entity* getEntity(void) { return playerEnt; }
	PlayerState* getPlayerState(void) { return &playerState; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	bool isCollidingWith(Player* other, Ogre::Vector3& pushedPosition) { 
		Ogre::Vector3 dist = other->positionNode->getPosition() - positionNode->getPosition();
		bool collide = dist.squaredLength() < 256; // distance < 16
		dist.normalise();
		pushedPosition = other->positionNode->getPosition() + dist*(-16);
		return collide;
	}
	bool isCollidingWith(Player* other) { 
		return positionNode->getPosition().squaredDistance(
			other->positionNode->getPosition()
		) < 256; // distance < 16
	}
	void handleKeyPressed(const OIS::KeyCode key);
	void handleKeyReleased(const OIS::KeyCode key);
	void handleMouseMoved( int dx, int dy );
	void handleMousePressed( int x, int y, OIS::MouseButtonID id );
	void handleMouseReleased( int x, int y, OIS::MouseButtonID id );
	bool isDead(void) { return playerState.hp == 0; }
	int getHP(void) { return playerState.hp; }
	void hitBy(PlayerAttack n) {
		playerState.hp -= n;
		playerState.hp = playerState.hp < 0 ? 0 : playerState.hp;
		switch (n) {
		case ATTACK_BLADE:
			soundHandler->play_sound(sword_hit); break;
		case ATTACK_KICK:
			soundHandler->play_sound(hyah); break;
		case ATTACK_SHURIKEN:
			//std::cout <<"  play sound - hit by shuriken" << std::endl; break;
			soundHandler->play_sound(grunt);
		}
		if(isAI) {
			hpbar->resetDimensions();
			hpbar->setDimensions(hpbar->mParentSet->getDefaultWidth()*((float)playerState.hp / (float)playerState.defaultHP), hpbar->mParentSet->getDefaultHeight());
		} else {
			CEGUI::WindowManager::getSingleton().getWindow("Pluto/PlanetRoot/HPBar")->setWidth(CEGUI::UDim(((float)playerState.hp / (float)playerState.defaultHP)*0.22, 0));
		}
	}
	void switchWeapon(void) {
		CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
		switch(playerState.weapon) {
		case WEAPON_BLADE:
			playerState.weapon = WEAPON_SHURIKEN;
			playerEnt->getSubEntity(1)->setVisible(false);
			wmgr->getWindow("Pluto/PlanetRoot/Blade")->setVisible(false);
			wmgr->getWindow("Pluto/PlanetRoot/Shuriken")->setVisible(true);
			wmgr->getWindow("Pluto/PlanetRoot/ShurikenCount")->setVisible(true);
			sprintf(shuriken_buf, "x%d", playerState.numShuriken);
			wmgr->getWindow("Pluto/PlanetRoot/ShurikenCount")->setText(shuriken_buf);
			break;
		case WEAPON_SHURIKEN:
			playerState.weapon = WEAPON_BLADE;
			playerEnt->getSubEntity(1)->setVisible(true);
			wmgr->getWindow("Pluto/PlanetRoot/Blade")->setVisible(true);
			wmgr->getWindow("Pluto/PlanetRoot/Shuriken")->setVisible(false);
			wmgr->getWindow("Pluto/PlanetRoot/ShurikenCount")->setVisible(false);
			break;
		}
	}
	void attack(void) {
		if (playerState.action == BLOCK || playerState.action == STOP_BLOCK) {
			playerEnt->getAnimationState("Block")->setTimePosition(0);
			playerEnt->getAnimationState("Block")->setEnabled(false);
		} else if (playerState.action != IDLE) return;
		playerState.action = ATTACK;
		playerEnt->getAnimationState("Attack3")->setEnabled(true);
		attackEffectChecked = false;
	}
	void block(void) {
		if (playerState.action != IDLE) return;
		playerState.action = BLOCK;
		playerEnt->getAnimationState("Block")->setEnabled(true);
	}
	void stopBlock(void) {
		if (playerState.action == BLOCK) playerState.action = STOP_BLOCK;
	}

	void throwShuriken(void) {
		if (playerState.action != IDLE) return;
		playerState.action = THROW_SHURIKEN;
		playerEnt->getAnimationState("Spin")->setEnabled(true);
		attackEffectChecked = false;
		isPerformingShurikenAOE = false;
	}
	void shurikenAOE(void) {
		if (playerState.action != IDLE) return;
		throwShuriken();
		isPerformingShurikenAOE = true;
	}
	void jump(void) {
		if (playerState.action != IDLE) return;
		playerState.action = JUMP;
		playerEnt->getAnimationState("Jump")->setEnabled(true);
	}
	void kick(void) {
		if (playerState.action != IDLE) return;
		playerState.action = KICK;
		attackEffectChecked = false;
		playerEnt->getAnimationState("SideKick")->setEnabled(true);
	}
	void kicked(void) {
		playerState.action = KICKED;
		playerEnt->getAnimationState("Attack3")->setTimePosition(0);
		playerEnt->getAnimationState("Attack3")->setEnabled(false);
		playerEnt->getAnimationState("Block")->setTimePosition(0);
		playerEnt->getAnimationState("Block")->setEnabled(false);
		playerEnt->getAnimationState("Jump")->setTimePosition(0);
		playerEnt->getAnimationState("Jump")->setEnabled(false);
		playerEnt->getAnimationState("SideKick")->setTimePosition(0);
		playerEnt->getAnimationState("SideKick")->setEnabled(false);
		playerEnt->getAnimationState("Backflip")->setEnabled(true);
		playerEnt->getAnimationState("Spin")->setTimePosition(0);
		playerEnt->getAnimationState("Spin")->setEnabled(false);
	}
	void die(void) {
		playerState.hp = 0;
		playerState.action = DIE;
		playerEnt->getAnimationState("Backflip")->setEnabled(false);
		playerEnt->getAnimationState("Attack3")->setEnabled(false);
		playerEnt->getAnimationState("Block")->setEnabled(false);
		playerEnt->getAnimationState("Jump")->setEnabled(false);
		playerEnt->getAnimationState("SideKick")->setEnabled(false);
		playerEnt->getAnimationState("Death1")->setEnabled(true);
	}
};
#endif // #ifndef __Player_h_
