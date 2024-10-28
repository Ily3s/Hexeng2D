#include "Animation.hpp"
#include "Renderer.hpp"

namespace Hexeng::Renderer
{

	AnimationStep::AnimationStep(std::function<void(float)> up_anim_p, std::function<void(void)> keypoint_p, float timestamp_p)
		: up_anim(up_anim_p),
		keypoint(keypoint_p),
		timestamp(timestamp_p) {}

	Animation::Animation(std::vector<AnimationStep> steps_p, std::function<void(void)> terminate_p, bool repeat_p)
		: steps(steps_p),
		terminate(terminate_p),
		repeat(repeat_p) {}

	std::vector<Animation*> Animation::s_animations;

	void Animation::play()
	{
		pending_actions.push_back([this]() {
			s_animations.push_back(this);
			});
	}

	void Animation::stop()
	{
		pending_actions.push_back([this]() {
			auto it = std::find(s_animations.begin(), s_animations.end(), this);
			if (it != s_animations.end())
				s_animations.erase(it);
			terminate();
			m_current_step = 0;
			m_time = 0.0f;
			});
	}

	void Animation::s_update_animations()
	{
		for (Animation* anim : Animation::s_animations)
		{
			anim->m_time += frame_time;
			if (anim->m_time >= anim->steps[anim->m_current_step].timestamp)
			{
				anim->steps[anim->m_current_step].up_anim(frame_time - (anim->m_time - anim->steps[anim->m_current_step].timestamp));
				anim->steps[anim->m_current_step].keypoint();
				anim->m_time -= anim->steps[anim->m_current_step].timestamp;
				anim->m_current_step++;
				if (anim->m_current_step >= anim->steps.size())
				{
					if (anim->repeat)
					{
						anim->m_current_step = 0;
					}
					else
					{
						auto it = std::find(s_animations.begin(), s_animations.end(), anim);
						if (it != s_animations.end())
							s_animations.erase(it);
						anim->terminate();
						anim->m_current_step = 0;
						anim->m_time = 0.0f;
						continue;
					}
				}
				anim->steps[anim->m_current_step].up_anim(anim->m_time);
				continue;
			}
			anim->steps[anim->m_current_step].up_anim(frame_time);
		}
	}

}