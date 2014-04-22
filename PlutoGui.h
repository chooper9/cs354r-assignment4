#ifndef __PlutoGui_h_
#define __PlutoGui_h_

#include "GameResource.h"
#include "Pluto.h"

void Pluto::setup_PlutoGui() {
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	//CEGUI::MouseCursor::getSingleton().hide();

	// GUI imageset(s)
	CEGUI::Imageset *PlutoImageset = &CEGUI::ImagesetManager::getSingleton().create("Pluto.imageset");

	// GUI layout
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "Game/Sheet");
	CEGUI::System::getSingleton().setGUISheet(sheet);
	CEGUI::Window *plutoGuiRoot = wmgr.loadWindowLayout("Pluto.layout");
	sheet->addChildWindow(plutoGuiRoot);
	wmgr.getWindow("Pluto/PlanetRoot/HPBar")->moveToBack();
	
	// Animation
	CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("PlutoAnimations.xml");
	CEGUI::Animation* anim = CEGUI::AnimationManager::getSingleton().getAnimation("Slide_in");
	CEGUI::AnimationInstance* anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim); // indx 0
	anmtn->setTargetWindow(wmgr.getWindow("Pluto/PauseRoot/OptionsRoot"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim); // indx 1
	anmtn->setTargetWindow(wmgr.getWindow("Pluto/OptionsRoot/SoundOptions"));

	anim = CEGUI::AnimationManager::getSingleton().getAnimation("Slide_out");
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim); // indx 2
	anmtn->setTargetWindow(wmgr.getWindow("Pluto/PauseRoot/OptionsRoot"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim); // indx 3
	anmtn->setTargetWindow(wmgr.getWindow("Pluto/OptionsRoot/SoundOptions"));
	

	// subscribe events
	wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::quitGame, this));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::resumeGame, this));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showOptions, this));
	wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu/Sounds")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showSoundOptions, this));
	wmgr.getWindow("Pluto/TitleRoot/StartGame")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::startGame, this));
	wmgr.getWindow("Pluto/TitleRoot/Controls")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showControls, this));
	wmgr.getWindow("Pluto/TitleRoot/Exit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::quitGame, this));
	wmgr.getWindow("Pluto/ControlRoot/NextPage")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::controls_togglePage, this));
	wmgr.getWindow("Pluto/ControlRoot/PrevPage")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::controls_togglePage, this));
	wmgr.getWindow("Pluto/ControlRoot/Return")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::controls_return, this));
}

bool Pluto::resumeGame(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/PauseRoot")->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	return true;
}

bool Pluto::quitGame(const CEGUI::EventArgs &e) {
	mShutDown = true;
	return true;
}

bool Pluto::showOptions(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	if(wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu")->isDisabled()) {
		wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu")->enable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->disable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->disable();
		CEGUI::AnimationManager::getSingleton().getAnimationInstanceAtIdx(2)->start();
	} else {
		wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu")->disable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->enable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->enable();
		CEGUI::AnimationManager::getSingleton().getAnimationInstanceAtIdx(0)->start();
	}
	return true;
}

// enable/disable other options
bool Pluto::showSoundOptions(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	if(wmgr.getWindow("Pluto/OptionsRoot/SoundOptions")->isDisabled()) {
		wmgr.getWindow("Pluto/OptionsRoot/SoundOptions")->enable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->disable();
		CEGUI::AnimationManager::getSingleton().getAnimationInstanceAtIdx(3)->start();
	} else {
		wmgr.getWindow("Pluto/OptionsRoot/SoundOptions")->disable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->enable();
		CEGUI::AnimationManager::getSingleton().getAnimationInstanceAtIdx(1)->start();
	}
	return true;
}

bool Pluto::startGame(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/TitleRoot")->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	return true;
}

bool Pluto::showControls(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/TitleRoot")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot")->setVisible(true);
	return true;
}

bool Pluto::controls_togglePage(const CEGUI::EventArgs &e) {
	bool page1Vis = CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot/Page1")->isVisible();
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot/Page1")->setVisible(!page1Vis);
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot/Page2")->setVisible(page1Vis);
	return true;
}

bool Pluto::controls_return(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot/Page1")->setVisible(true);
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot/Page2")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/ControlRoot")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/TitleRoot")->setVisible(true);
	return true;
}

#endif