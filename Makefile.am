ACLOCAL_AMFLAGS= -I m4

noinst_HEADERS= PhysicsObject.h PhysicsEngine.h Sound.h GameResource.h Shuriken.h Player.h Planet.h Scene.h ScenePlanet.h SceneSpace.h Game.h BaseApplication.h Pluto.h PlutoGui.h SceneTerrain.h TerrainMaterial.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= PhysicsObject.cpp PhysicsEngine.cpp Shuriken.cpp Player.cpp Planet.cpp BaseApplication.cpp Scene.cpp ScenePlanet.cpp SceneSpace.cpp Pluto.cpp Game.cpp Sound.cpp SceneTerrain.cpp TerrainMaterial.cpp

OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_net_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDL_mixer_LIBS) $(SDL_net_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign

