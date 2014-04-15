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
	CEGUI::MouseCursor::getSingleton().hide();

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
	CEGUI::AnimationInstance* anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	anmtn->setTargetWindow(wmgr.getWindow("Pluto/PauseRoot/OptionsMenu"));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CEGUI::AnimationInstance::handleStart, anmtn));
	
	// subscribe events
	wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::quitGame, this));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::resumeGame, this));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showOptions, this));
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
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/PauseRoot/OptionsMenu")->setVisible(true);
	return true;
}

#endif