#include "Animator.h"

Animator::Animator(Texture2D& _targetTexture, std::map<std::string, Animation> _animations, std::string _defaultAnimation, std::string _characterDirectory) : targetTexture(_targetTexture)
{
	animations = _animations;
	defaultAnimation = _defaultAnimation;
	characterDirectory = _characterDirectory;
	currentAnimation = _defaultAnimation;
}

void Animator::Update(float deltaTime)
{
	timer += deltaTime;

	if (timer >= animations[currentAnimation].delay * delayModifier)
	{
		timer = 0.0f;
		currentFrame += 1;

		if (currentFrame == animations[currentAnimation].framePaths.size())
		{
			currentFrame = 0;

			if (!animations[currentAnimation].looping)
			{
				SetAnimation(defaultAnimation);
			}
		}

		targetTexture.LoadFromFile(characterDirectory + animations[currentAnimation].framePaths[currentFrame]);
	}
}

void Animator::SetAnimation(std::string targetAnimation, int startFrame, float _delayModifier)
{
	if (currentAnimation != targetAnimation)
	{
		currentAnimation = targetAnimation;
		currentFrame = startFrame;
		delayModifier = _delayModifier;

		timer = 0.0f;
		targetTexture.LoadFromFile(characterDirectory + animations[currentAnimation].framePaths[currentFrame]);
	}
}
