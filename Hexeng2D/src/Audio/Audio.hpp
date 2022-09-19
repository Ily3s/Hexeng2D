#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <cassert>
#include <vector>
#include <memory>

#include "PortAudio/portaudio.h"

#include "../Macros.hpp"
#include "../Vectors.hpp"

namespace Hexeng::Audio
{

	void HXG_DECLSPEC init();
	void HXG_DECLSPEC terminate();

	class HXG_DECLSPEC SoundBase
	{
	public:

		SoundBase() = default;

		SoundBase(std::string filepath);

		SoundBase(const SoundBase&) = delete;
		SoundBase& operator=(const SoundBase&) = delete;

		SoundBase(SoundBase&&) noexcept;
		SoundBase& operator=(SoundBase&&) noexcept;

		virtual void terminate() = 0;

	protected:

		std::string m_file_buffer;
		size_t m_byte_per_sample = 0;
		uint16_t m_channels_nb = 0;
		uint32_t m_sample_rate = 0;

		friend void clean_garbages();
		std::vector<std::thread> m_garbage;
	};

	extern HXG_DECLSPEC const Vec2<int>* receiver;

	struct SoundData
	{
		void* sound;
		PaStream* stream;
		float* buffer;
		const Vec2<int>* emitter;
		bool has_to_stop;
		bool is_playing;

		SoundData(void* sound_p, PaStream* stream_p, float* buffer_p,const Vec2<int>* emitter_p = nullptr,
			bool has_to_stop_p = false, bool is_playing_p = true) :
			sound(sound_p), stream(stream_p), buffer(buffer_p),
			has_to_stop(has_to_stop_p), is_playing(is_playing_p), emitter(emitter_p)
		{}
	};

	class HXG_DECLSPEC Sound : public SoundBase
	{
	public :

		Sound() = default;

		Sound(std::string filepath);

		Sound(const Sound&) = delete;
		Sound& operator=(const Sound&) = delete;

		Sound(Sound&&) noexcept;
		Sound& operator=(Sound&&) noexcept;

		void play(const Vec2<int>* emmitter);
		void stop();

		static int audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		void terminate() override;

	private :

		std::vector<std::unique_ptr<SoundData>> m_sounds_data;

		void m_stop_streams();
		std::thread m_thread;
		bool m_has_to_end = false;
	};

	class HXG_DECLSPEC StaticSound : public SoundBase
	{
	public:

		StaticSound() = default;

		StaticSound(std::string filepath);

		StaticSound(const StaticSound&) = delete;
		StaticSound& operator=(const StaticSound&) = delete;

		StaticSound(StaticSound&&) noexcept;
		StaticSound& operator=(StaticSound&&) noexcept;

		void play();
		void stop();

		static int audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		void terminate() override;

	private:

		std::vector<std::unique_ptr<SoundData>> m_sounds_data;

		void m_stop_streams();
		std::thread m_thread;
		bool m_has_to_end = false;
	};

	class HXG_DECLSPEC Music : public SoundBase
	{
	public:

		Music() = default;

		Music(std::string filepath);

		Music(const Music&) = delete;
		Music& operator=(const Music&) = delete;

		Music(Music&&) noexcept;
		Music& operator=(Music&&) noexcept;

		void play();
		void stop();

		static int audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		void terminate() override;

	private:

		PaStream* m_stream = nullptr;
		float* m_buffer = nullptr;
		bool m_is_playing = false;
	};

}

#endif