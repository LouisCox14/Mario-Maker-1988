#include "AudioPlayer.h"
#include <iostream>

AudioPlayer::AudioPlayer(std::vector<std::string> _soundClips, std::vector<std::string> _musicTracks)
{
	for (std::string clipName : _soundClips)
	{	
		Mix_Chunk* soundClip = Mix_LoadWAV(("Audio/" + clipName + ".wav").c_str());
		if (soundClip)
		{
			soundClips.insert({ clipName, soundClip });
		}
		else
		{
			std::cout << "Error loading sound clip " << "Audio/" + clipName + ".wav" << std::endl;
		}
	}

	for (std::string trackName : _musicTracks)
	{
		Mix_Music* musicTrack = Mix_LoadMUS(("Audio/" + trackName + ".wav").c_str());
		if (musicTrack)
		{
			musicTracks.insert({ trackName, musicTrack });
		}
		else
		{
			std::cout << "Error loading music track " << "Audio/" + trackName + ".wav" << std::endl;
		}
	}
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::PlayClip(std::string clipName)
{
	if (soundClips.find(clipName) != soundClips.end())
	{
		Mix_PlayChannel(-1, soundClips.at(clipName), 0);
	}
	else
	{
		std::cout << clipName << " not found." << std::endl;
	}
}

void AudioPlayer::PlayMusic(std::string trackName)
{
	if (musicTracks.find(trackName) != musicTracks.end())
	{
		if (currentTrack != trackName)
		{
			Mix_PlayMusic(musicTracks.at(trackName), -1);
			currentTrack = trackName;
		}
	}
	else
	{
		std::cout << trackName << " not found." << std::endl;
	}
}

void AudioPlayer::StopMusic()
{
	if (Mix_PlayingMusic() == 1)
	{
		Mix_PauseMusic();
	}
}
