#pragma once
#include "Texture2D.h"
#include <SDL.h>
#include <vector>
#include <string>
#include <map>

struct Animation
{
	std::vector<std::string> framePaths;
	float delay;
	bool looping;

	Animation(std::vector<std::string> _framePaths = {}, float _delay = 0.0f, bool _looping = false)
	{
		framePaths = _framePaths;
		delay = _delay;
		looping = _looping;
	}
};

class Animator
{
	public:
		Texture2D& targetTexture;

		std::map<std::string, Animation> animations;
		std::string currentAnimation;
		int currentFrame;
		float timer;
		float delayModifier = 1.0f;

		std::string defaultAnimation;
		std::string characterDirectory;

		Animator(Texture2D& _targetTexture, std::map<std::string, Animation> _animations, std::string _defaultAnimation, std::string _characterDirectory);
		void Update(float deltaTime);
		void SetAnimation(std::string targetAnimation, int startFrame = 0, float _delayModifier = 1.0f);
};