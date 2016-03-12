#include "AudioManager.hpp"

AudioManager::AudioManager() {

	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(maxChannels);
	channels.resize(maxChannels, 0);

	musicTracks.resize(MUSIC_TRACK_LAST);
	musicTracks[DRIPPY_EYE] = "./resources/drippy_eye.mp3";
	musicTracks[MOTORLICKER] = "./resources/motorlicker.mp3";

	soundEffects.resize(SOUND_EFFECT_LAST);
	soundEffects[BIG_ROPE_CREEK] = { "./resources/big_rope_creek.wav" };
	soundEffects[ROPE_PULL]      = { "./resources/rope_pull.wav" };
	soundEffects[ROPE_RETRACT]   = { "./resources/rope_retract.wav" };
	soundEffects[RUSTY_SWING]    = { "./resources/rusty_swing.wav" };
	soundEffects[ROPE_HIT]       = { "./resources/whip.wav" };
	soundEffects[ROPE_MISS]      = { "./resources/rope_pull.wav" };
	soundEffects[ROCKET_RUMBLE]  = { "./resources/rocket_rumble.wav" };

	for (unsigned int i = 0; i < soundEffects.size(); ++i) {
		soundEffects[i].chunk = Mix_LoadWAV(soundEffects[i].filename.c_str());
	}
}

void AudioManager::playMusicTrack(MusicTrackId musicTrackId) {
	Mix_VolumeMusic(32);
	Mix_Music *music = Mix_LoadMUS(musicTracks[musicTrackId].c_str());
	Mix_PlayMusic(music, -1);
}

unsigned int AudioManager::playSoundEffect(SoundEffectId effectId, int repetition) {

	// get first available channel
	int channel = -1;
	for (unsigned int i = 0; i < maxChannels; ++i) {
		if (channels[i] == 0)
			channel = i;
	}

	if (channel != -1) {
		Mix_Chunk* chunk = soundEffects[effectId].chunk;
		Mix_PlayChannel(channel, chunk, repetition);
		channels[channel] = effectCounter;
		effectInstanceChannels[effectCounter] = channel;

		return effectCounter++;
	}

	return 0;
}

void AudioManager::stopSoundEffect(unsigned int soundEffectInstanceId) {
	
	unsigned int channel = effectInstanceChannels[soundEffectInstanceId];
	Mix_HaltChannel(channel);
	channels[channel] = 0; // mark as free to be used again
	effectInstanceChannels.erase(soundEffectInstanceId);

}

void AudioManager::setSoundEffectsVolume(unsigned int volume) {
	Mix_Volume(-1, volume);
}

AudioManager::~AudioManager() {

	Mix_HaltChannel(-1);
	Mix_HaltMusic();

	for (unsigned int i = 0; i < soundEffects.size(); ++i) {
		Mix_FreeChunk(soundEffects[i].chunk);
	}

	Mix_FreeMusic(music);
	Mix_CloseAudio();
	Mix_Quit();
}
