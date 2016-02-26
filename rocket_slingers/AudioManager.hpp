#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <sdl.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <vector>

class AudioManager
{
public:

	enum MusicTrackId {
		DRIPPY_EYE,
		MOTORLICKER,
		MUSIC_TRACK_LAST
	};

	enum SoundEffectId {
		BIG_ROPE_CREEK,
		ROPE_PULL,
		ROPE_RETRACT,
		RUSTY_SWING,
		WHIP,
		ROCKET_RUMBLE,
		SOUND_EFFECT_LAST
	};

	AudioManager();
	void playMusicTrack(MusicTrackId musicTrackId);
	unsigned int playSoundEffect(SoundEffectId effectId, int repetition);
	void stopSoundEffect(unsigned int soundEffectInstanceId);

	~AudioManager();

private:

	struct SoundEffect {
		std::string filename;
		Mix_Chunk* chunk;
	};

	Mix_Music *music;

	const unsigned int maxChannels = 256;
	std::vector<SoundEffect> soundEffects;
	std::vector<std::string> musicTracks;
	std::vector<unsigned int> channels;
	std::map<unsigned int, unsigned int> effectInstanceChannels;
	unsigned int effectCounter = 1;
};

#endif
