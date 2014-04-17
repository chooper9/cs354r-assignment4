
#include "Pluto.h"
#include "PlutoGui.h"
#include <OgreMath.h>
#include <OgreSphere.h>
#include <iostream>
#include <stdio.h>
//-------------------------------------------------------------------------------------
Pluto::Pluto(void) : 
	mPaused(true),
	mouseClicked(false),
	bounces(0),
	collisions(0),
	mRotate(0.15),
	mMove(250),
	mDirection(Ogre::Vector3::ZERO)
{
}

//-------------------------------------------------------------------------------------

Pluto::~Pluto(void)
{
	delete game;
	delete soundHandler;
	std::cout << "========= Debug: Pluto Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void Pluto::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MainCam");
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(1);
}
//-------------------------------------------------------------------------------------

void Pluto::createScene(void)
{
	setup_PlutoGui();

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mPaused =true;

	game = new Game(mSceneMgr);

	soundHandler = new Sound();
	soundHandler->start_ambient();
}

//-------------------------------------------------------------------------------------

void Pluto::createFrameListener(void) {
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
bool Pluto::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed()) return false;
	if (mShutDown) return false;
	mKeyboard->capture();
	mMouse->capture();
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	//mCamNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	game->runNextFrame(evt);
	return true;
}
//-------------------------------------------------------------------------------------
// OIS::KeyListener
bool Pluto::keyPressed( const OIS::KeyEvent& evt ){
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(evt.key);
	sys.injectChar(evt.text);

	if(evt.key == OIS::KC_ESCAPE) {
		mShutDown = true;
		return true;
	}
	game->handleKeyPressed(evt.key);
	return true;
}
//-------------------------------------------------------------------------------------
bool Pluto::keyReleased( const OIS::KeyEvent& evt ){
	CEGUI::System::getSingleton().injectKeyUp(evt.key);
	game->handleKeyReleased(evt.key);
	return true;
}
//-------------------------------------------------------------------------------------

// OIS::MouseListener
bool Pluto::mouseMoved( const OIS::MouseEvent& evt ){
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	if(evt.state.Z.rel)
		sys.injectMouseWheelChange(evt.state.Z.rel / 120.0f);

	game->handleMouseMoved(evt.state.X.rel, evt.state.Y.rel);
	//mCamNode->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
    	//mCamNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
	return true;
}
//-------------------------------------------------------------------------------------
bool Pluto::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	game->handleMousePressed(evt.state.X.abs, evt.state.Y.abs, id);
	return true;
}

//-------------------------------------------------------------------------------------

bool Pluto::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	game->handleMouseReleased(evt.state.X.abs, evt.state.Y.abs, id);
	return true;
}

//--------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Pluto app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
