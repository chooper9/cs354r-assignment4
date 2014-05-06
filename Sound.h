/*
Anthony Garza 
Sound.h
created: 4/17/2014
*/

#ifndef __Sound_h_
#define __Sound_h_


#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

//CONSTANTS TO IDENTIFY EACH SOUND
static const char *ambient_sound = "./media/sounds/spring-weather-1.wav";
static const char *grunt = "./media/sounds/grunt.wav";
static const char *hyah = "./media/sounds/hyah.wav";
static const char *explosion = "./media/sounds/explosion.wav";
static const char *sword_miss = "./media/sounds/sword_swipe.wav";
static const char *sword_hit = "./media/sounds/sword_slice_flesh.wav";
static const char *sword_block = "./media/sounds/sword_block.wav";
static const char *toglory = "./media/sounds/toglory.wav";
static const char *bossbattle = "./media/sounds/pokemonrockremix.wav";
static const char *win = "./media/sounds/tada.wav";
static const char *fireworks = "./media/sounds/fireworks.wav";
static const char *pluto_dead = "./media/sounds/pluto_dead.wav";

static bool sound_effects = true;

class Sound{
public:
	Sound(void);
	~Sound(void);
	void play_sound(const char*);
	void start_ambient(const char* track = ambient_sound);
	void set_ambient_volume(int chan, int vol);
	void set_effects_volume(int vol);
	void stop_ambient();
	void fade_out_music();
	void play_explosion();
};

static SDL_AudioSpec s_wanted;
static Mix_Chunk *sound_c; 
static Mix_Music *ambient_chunk;
static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;
static int channel, a_channel, p_u_chan, effects_vol = 64, amb_vol = 64;
#endif
