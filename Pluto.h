
#ifndef __Pluto_h_
#define __Pluto_h_

#include "BaseApplication.h"
#include "GameResource.h"
#include "Game.h"
#include <CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <stdio.h>


class Pluto : public BaseApplication
{

public:
    Pluto(void);
    virtual ~Pluto(void);
protected:
	const static Ogre::Real BOX_SIDE_LENGTH = 200.0f;
	Game* game;

Ogre::AnimationState* animation;
	bool mouseClicked;
	bool mPaused;
	Ogre::Real mRotate;
    	Ogre::Real mMove;
    	Ogre::Vector3 mDirection;

	// not used for now
	//OgreBites::ParamsPanel* scorePanel; 
	const static int MAX_NUM_BALLS = 10;
	bool pauses [MAX_NUM_BALLS];
	int bounces;
	int collisions;
	
	virtual void createCamera(void);
    	virtual void createScene(void);
	virtual void createFrameListener(void);
    	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    	virtual bool keyPressed( const OIS::KeyEvent& evt );
	virtual bool keyReleased( const OIS::KeyEvent& evt );
	virtual bool mouseMoved( const OIS::MouseEvent& evt );
	virtual bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
};

#endif // #ifndef __Pluto_h_
