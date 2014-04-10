#ifndef __Player_h_
#define __Player_h_

#include "GameResource.h"

class Player {
private:
	bool attackEffectChecked;
	Ogre::Vector3 orient;
	bool visible;
	bool isAI;
	void checkAttackEffect(Player* enemy);
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* playerEnt;
	Ogre::SceneNode* positionNode;
	PhysicsObject physicsObject;
	PhysicsEngine* physicsEngine;
	PlayerState playerState;
public:
	Player(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* parentNode, PhysicsEngine* bulletEngine, bool isPluto, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Player(void);
	void toggleVisible(void);
	void resetState(void);
	void runNextFrame(const Ogre::FrameEvent& evt);
	void reactTo(Player* enemy);
	void setPosition(const Ogre::Vector3& pos);
	Ogre::SceneNode* getSceneNode(void) { return positionNode; }
	Ogre::Entity* getEntity(void) { return playerEnt; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
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
	}

	void attack(void) {
		if (playerState.action == BLOCK || 
			playerState.action == STOP_BLOCK || 
			playerState.action == ATTACK) 
			return;
		playerState.action = ATTACK;
		playerEnt->getAnimationState("Attack3")->setEnabled(true);
		attackEffectChecked = false;
	}
	void block(void) {
		if (playerState.action == ATTACK) return;
		playerState.action = BLOCK;
		playerEnt->getAnimationState("Block")->setEnabled(true);
	}
	void stopBlock(void) {
		if (playerState.action == BLOCK) playerState.action = STOP_BLOCK;
	}
	void jump(void) {
		if (playerState.action == KICK) return;
		playerState.action = JUMP;
		playerEnt->getAnimationState("Jump")->setEnabled(true);
	}
	void kick(void) {
		if (playerState.action == JUMP) return;
		playerState.action = KICK;
		attackEffectChecked = false;
		playerEnt->getAnimationState("Attack3")->setEnabled(false);
		playerEnt->getAnimationState("Block")->setEnabled(false);
		playerEnt->getAnimationState("Jump")->setEnabled(false);
		playerEnt->getAnimationState("Kick")->setEnabled(true);
	}
	void die(void) {
		playerState.hp = 0;
		playerState.action = DIE;
		playerEnt->getAnimationState("Attack3")->setEnabled(false);
		playerEnt->getAnimationState("Block")->setEnabled(false);
		playerEnt->getAnimationState("Jump")->setEnabled(false);
		playerEnt->getAnimationState("Kick")->setEnabled(false);
		playerEnt->getAnimationState("Death1")->setEnabled(true);
	}
};
#endif // #ifndef __Player_h_
