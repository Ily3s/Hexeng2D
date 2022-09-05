#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <functional>

#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	struct HXG_DECLSPEC AnimationStep
	{
		AnimationStep(std::function<void(float)> up_anim, std::function<void(void)> keypoint, float timestamp);

		std::function<void(float)> up_anim;
		std::function<void(void)> keypoint;
		float timestamp;
	};

	class HXG_DECLSPEC Animation
	{
	public :

		Animation(std::vector<AnimationStep> steps, std::function<void(void)> terminate, bool repeat = false);

		std::vector<AnimationStep> steps;
		std::function<void(void)> terminate;
		bool repeat = false;

		void play();
		void stop();

		static std::vector<Animation*> animations;

		size_t current_step = 0;

		float time = 0.0f;

		static void update_animations();
	};

}

#endif