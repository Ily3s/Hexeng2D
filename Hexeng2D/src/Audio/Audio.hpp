#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <cassert>
#include <vector>
#include <memory>

#include "PortAudio/portaudio.h"

#include "../Macros.hpp"

namespace Hexeng::Audio
{

	extern PaError e;

#ifdef _DEBUG

#define HXG_PA(x) \
e = x;\
if(e != paNoError)\
{std::cout << Pa_GetErrorText(e) << std::endl;\
 assert(e == paNoError);}""

#else

#define HXG_PA(x) x

#endif

	void HXG_DECLSPEC init();
	void HXG_DECLSPEC terminate();

	class Sound;

	struct SoundData
	{
		Sound* sound;
		PaStream* stream;
		char* buffer;
		bool has_to_stop;
		bool is_playing;

		SoundData(Sound* first, PaStream* second, char* third, bool fourth, bool fifth) :
			sound(first), stream(second), buffer(third), has_to_stop(fourth), is_playing(fifth)
		{}
	};

	class HXG_DECLSPEC Sound
	{
	public :

		Sound() = default;

		Sound(std::string filepath);

		Sound(const Sound&) = delete;
		Sound& operator=(const Sound&) = delete;

		Sound(Sound&&) noexcept;
		Sound& operator=(Sound&&) noexcept;

		void play();
		void stop();

		static int audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		void terminate();

	private :

		std::vector<std::unique_ptr<SoundData>> m_sounds_data;

		void m_stop_streams();
		std::thread m_thread;
		bool m_has_to_end = false;

		std::string m_file_buffer;
		size_t m_byte_per_sample = 0;
		uint16_t m_channels_nb = 0;
		uint32_t m_sample_rate = 0;
		PaSampleFormat m_format = 0;
	};

	std::vector<Sound*> sounds;

}

#endif