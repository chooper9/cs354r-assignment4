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
	

	std::string storyText ("");
	storyText += " \nAll was peaceful in the solar system...\n \n";
	storyText += "Until one day some famous Earth scientist shook the foundations of the Cosmos.\n \n";
	storyText += "\"PLUTO IS NOT A PLANET\" he said, unaware of the consequences to come.\n \n";
	storyText += "His words, reverberating through space, finally reach the distant Pluto... and Pluto is pissed.\n \n";
	storyText += "Now, donning the spirit of a brave ninja warrior, Pluto makes his way towards Earth -\n \n";
	storyText += "destroying every planet that stands in his way - to exact revenge upon the misled astrophysicist.\n \n";
	wmgr.getWindow("Pluto/StoryRoot/Text")->setText(storyText);

	// subscribe events
	
	wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::quitGame, this));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::resumeGame, this));
	wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showOptions, this));
	wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu/Sounds")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showSoundOptions, this));
	wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu/Sounds/BGMVolume")->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&Pluto::options_set_BGMvolume, this));
	wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu/Sounds/SFXVolume")->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&Pluto::options_set_SFXvolume, this));
	wmgr.getWindow("Pluto/TitleRoot/StartGame")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::startGame, this));
	wmgr.getWindow("Pluto/StoryRoot/Continue")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::startGame2, this));
	wmgr.getWindow("Pluto/TitleRoot/Controls")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::showControls, this));
	wmgr.getWindow("Pluto/TitleRoot/Exit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::quitGame, this));
	wmgr.getWindow("Pluto/ControlRoot/NextPage")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::controls_togglePage, this));
	wmgr.getWindow("Pluto/ControlRoot/PrevPage")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::controls_togglePage, this));
	wmgr.getWindow("Pluto/ControlRoot/Return")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::controls_return, this));
	wmgr.getWindow("Pluto/GameOverRoot/No")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::quitGame, this));
	wmgr.getWindow("Pluto/GameOverRoot/Yes")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Pluto::game_over_retry, this));
}

bool Pluto::resumeGame(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/PauseRoot")->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	game->unPause();
	return true;
}

bool Pluto::quitGame(const CEGUI::EventArgs &e) {
	mShutDown = true;
	return true;
}

bool Pluto::showOptions(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::AnimationManager &anmgr = CEGUI::AnimationManager::getSingleton();
	static CEGUI::AnimationInstance* animIn1 = anmgr.instantiateAnimation(anmgr.getAnimation("Slide_in"));
	animIn1->setTargetWindow(wmgr.getWindow("Pluto/PauseRoot/OptionsRoot"));
	static CEGUI::AnimationInstance* animIn2 = anmgr.instantiateAnimation(anmgr.getAnimation("Slide_out"));
	animIn2->setTargetWindow(wmgr.getWindow("Pluto/PauseRoot/OptionsRoot"));
	if(wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu")->isDisabled()) {
		wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu")->enable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->disable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->disable();
		animIn1->start();
	} else {
		wmgr.getWindow("Pluto/OptionsRoot/OptionsMenu")->disable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Resume")->enable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Quit")->enable();
		animIn2->start();
	}
	return true;
}

// enable/disable other options
bool Pluto::showSoundOptions(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::AnimationManager &anmgr = CEGUI::AnimationManager::getSingleton();
	static CEGUI::AnimationInstance* animIn1 = anmgr.instantiateAnimation(anmgr.getAnimation("Slide_in"));
	animIn1->setTargetWindow(wmgr.getWindow("Pluto/OptionsRoot/SoundOptions"));
	static CEGUI::AnimationInstance* animIn2 = anmgr.instantiateAnimation(anmgr.getAnimation("Slide_out"));
	animIn2->setTargetWindow(wmgr.getWindow("Pluto/OptionsRoot/SoundOptions"));
	if(wmgr.getWindow("Pluto/OptionsRoot/SoundOptions")->isDisabled()) {
		wmgr.getWindow("Pluto/OptionsRoot/SoundOptions")->enable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->disable();
		animIn1->start();
	} else {
		wmgr.getWindow("Pluto/OptionsRoot/SoundOptions")->disable();
		wmgr.getWindow("Pluto/PauseRoot/Menu/Options")->enable();
		animIn2->start();
	}
	return true;
}

bool Pluto::options_set_BGMvolume(const CEGUI::EventArgs &e) {
	CEGUI::Scrollbar *vol = (CEGUI::Scrollbar*) CEGUI::WindowManager::getSingleton().getWindow("Pluto/OptionsRoot/OptionsMenu/Sounds/BGMVolume");
	soundHandler->set_ambient_volume(a_channel, (vol->getScrollPosition() * 128));
	return true;
}

bool Pluto::options_set_SFXvolume(const CEGUI::EventArgs &e) {
	CEGUI::Scrollbar *vol = (CEGUI::Scrollbar*) CEGUI::WindowManager::getSingleton().getWindow("Pluto/OptionsRoot/OptionsMenu/Sounds/SFXVolume");
	soundHandler->set_effects_volume(vol->getScrollPosition() * 128);
	return true;
}

bool Pluto::startGame(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/TitleRoot")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/StoryRoot")->setVisible(true);
	return true;
}

bool Pluto::startGame2(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/StoryRoot")->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	game->unPause();
	game->enterScene(SCENE_SPACE);
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

bool Pluto::game_over_retry(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("Pluto/GameOverRoot")->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	game->exitScene();
	game->enterScene(SCENE_PLANET);
	//game->unPause();
	return true;
}

#endif
