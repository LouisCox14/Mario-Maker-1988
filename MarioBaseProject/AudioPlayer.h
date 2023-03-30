#pragma once
#include <SDL_mixer.h>
#include <Vector>
#include <Map>
#include <String>

class AudioPlayer
{
	public:
		AudioPlayer(std::vector<std::string> _soundClips = {}, std::vector<std::string> _musicTracks = {});
		~AudioPlayer();

		std::map<std::string, Mix_Chunk*> soundClips;
		std::map<std::string, Mix_Music*> musicTracks;

		std::string currentTrack;

		void PlayClip(std::string clipName);
		void PlayMusic(std::string trackName);
		void StopMusic();
};