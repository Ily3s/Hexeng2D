#include "Animation.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	AnimationStep::AnimationStep(std::function<void(float)> up_anim_p, std::function<void(void)> keypoint_p, float timestamp_p)
		: up_anim(up_anim_p), keypoint(keypoint_p), timestamp(timestamp_p) {}

	Animation::Animation(std::vector<AnimationStep> steps_p, std::function<void(void)> terminate_p, bool repeat_p)
		: steps(steps_p), terminate(terminate_p), repeat(repeat_p) {}

	std::vector<Animation*> Animation::animations;

	void Animation::play()
	{
		pending_actions.push_back([this]() {
			animations.push_back(this);
			});
	}

	void Animation::stop()
	{
		pending_actions.push_back([this]() {
			auto it = std::find(animations.begin(), animations.end(), this);
			if (it != animations.end())
			{
				animations.erase(it);
			}
			terminate();
			current_step = 0;
			time = 0.0f;
			});
	}

	void Animation::update_animations()
	{
		for (Animation* anim : Animation::animations)
		{
			anim->time += frame_time;
			if (anim->time >= anim->steps[anim->current_step].timestamp)
			{
				anim->steps[anim->current_step].up_anim(frame_time - (anim->time - anim->steps[anim->current_step].timestamp));
				anim->steps[anim->current_step].keypoint();
				anim->time -= anim->steps[anim->current_step].timestamp;
				anim->current_step++;
				if (anim->current_step >= anim->steps.size())
				{
					if (anim->repeat)
					{
						anim->current_step = 0;
					}
					else
					{
						auto it = std::find(animations.begin(), animations.end(), anim);
						if (it != animations.end())
							animations.erase(it);
						anim->terminate();
						anim->current_step = 0;
						anim->time = 0.0f;
						continue;
					}
				}
				anim->steps[anim->current_step].up_anim(anim->time);
				continue;
			}
			anim->steps[anim->current_step].up_anim(frame_time);
		}
	}

}