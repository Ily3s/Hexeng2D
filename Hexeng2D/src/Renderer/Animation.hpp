#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <functional>

#include "../Macros.hpp"

namespace Hexeng::Renderer
{

	struct HXG_DECLSPEC AnimationStep
	{
		/// <param name="up_anim">Gets executed every frame with the time since last frame (in ms) in parameter</param>
		/// <param name="keypoint">Gets executed at the end of the AnimationStep</param>
		/// <param name="timestamp">The time that will take this animation step in milliseconds</param>
		AnimationStep(std::function<void(float)> up_anim, std::function<void(void)> keypoint, float timestamp);

		std::function<void(float)> up_anim;
		std::function<void(void)> keypoint;
		float timestamp;
	};

	class HXG_DECLSPEC Animation
	{
	public :

		/// <param name="terminate">Gets executed at the end of the animation.</param>
		Animation(std::vector<AnimationStep> steps, std::function<void(void)> terminate, bool repeat = false);

		std::vector<AnimationStep> steps;
		std::function<void(void)> terminate;
		bool repeat = false;

		/// @brief starts the animation
		void play();

		/// @brief stops the animation
		void stop();

	private :

		friend HXG_DECLSPEC void draw_current_scene();

		static std::vector<Animation*> s_animations;

		size_t m_current_step = 0;

		float m_time = 0.0f;

		static void s_update_animations();
	};

}

#endif