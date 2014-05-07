#include "Sound.h"


Sound::Sound(void){
	//init SDL library
    SDL_Init(SDL_INIT_AUDIO);
	
	/*set audio format*/
	s_wanted.freq = 22050;
	s_wanted.format = AUDIO_S16SYS;
	s_wanted.channels = 2; // 1 = mono, 2 = stereo
	s_wanted.samples = 4096; // good low-latency value for callback
	s_wanted.userdata = NULL;
	/* Open the audio device, forcing the desired format */
	if (Mix_OpenAudio(s_wanted.freq, s_wanted.format, s_wanted.channels, s_wanted.samples) != 0){
		fprintf(stdout, "Couldn't open audio: %s\n", SDL_GetError());
	}
}

Sound::~Sound(void){
	Mix_HaltMusic();
	if(ambient_chunk != NULL)
		Mix_FreeMusic(ambient_chunk);
	Mix_CloseAudio();
	SDL_Quit();
}

void Sound::start_ambient(const char* track){
	std::cout << "starting ambient sound track \n";
	if(ambient_chunk != NULL)
		Mix_HaltMusic();
		Mix_FreeMusic(ambient_chunk);
	ambient_chunk = NULL;
	ambient_chunk = Mix_LoadMUS(track);
	Mix_VolumeMusic(amb_vol);
	if(ambient_chunk == NULL)
		fprintf(stdout, "Unable to load ambient wav file: %s \n", Mix_GetError());
	a_channel = Mix_PlayMusic(ambient_chunk, -1);
	if(a_channel == -1)
		fprintf(stdout, "Unable to play ambient wav file: %s \n", Mix_GetError());
}

void Sound::play_sound(const char* current)
{
//	std::cout << "in play_sound \n";
	if (!sound_effects)
		return;
	if (sound_c != NULL)
		Mix_FreeChunk(sound_c);
	sound_c = NULL;
	sound_c = Mix_LoadWAV(current);
	if(sound_c == NULL){
		fprintf(stdout, "Unable to load wav file: %s \n", Mix_GetError());
		return;
	}
	Mix_VolumeChunk(sound_c, effects_vol);
	channel = Mix_PlayChannel(-1, sound_c, 0);
	if(channel == -1)
		fprintf(stdout, "Unable to play wav file: %s \n", Mix_GetError());
}

void Sound::play_explosion(){
	if (!sound_effects)
		return;
	if (sound_c != NULL)
		Mix_FreeChunk(sound_c);
	sound_c = NULL;
	sound_c = Mix_LoadWAV(explosion);
	if(sound_c == NULL){
		fprintf(stdout, "Unable to load wav file: %s \n", Mix_GetError());
		return;
	}
	Mix_VolumeChunk(sound_c, 55);
	channel = Mix_PlayChannel(-1, sound_c, 0);
	if(channel == -1)
		fprintf(stdout, "Unable to play wav file: %s \n", Mix_GetError());
}

void Sound::set_ambient_volume(int chan, int vol)
{
		Mix_VolumeMusic(vol);
		amb_vol = vol;
}

void Sound::set_effects_volume(int vol){
	effects_vol = vol;
}

void Sound::stop_ambient(){
	Mix_HaltMusic();
}

void Sound::fade_out_music(){
	Mix_FadeOutMusic(1000);
	SDL_Delay(1000);
}