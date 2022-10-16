#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <cassert>
#include <vector>
#include <memory>
#include <thread>

#include "portaudio.h"

#include "../Macros.hpp"
#include "../Vectors.hpp"

namespace Hexeng::Audio
{

	void HXG_DECLSPEC init();
	void HXG_DECLSPEC terminate();

	/// <summary>
	/// Abstract Base class of Sound and Music.
	/// </summary>
	class HXG_DECLSPEC SoundBase
	{
	public:

		SoundBase() = default;

		SoundBase(std::string filepath);

		SoundBase(const SoundBase&) = delete;
		SoundBase& operator=(const SoundBase&) = delete;

		/// @note Expected other to be unused
		SoundBase(SoundBase&& other) noexcept;

		/// @note Expected other to be unused and this to be initialized by the default constructor
		SoundBase& operator=(SoundBase&& other) noexcept;

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

	/// <summary>
	/// A Sound that can be played multiple times at once, from a location in space.
	/// </summary>
	class HXG_DECLSPEC Sound : public SoundBase
	{
	public :

		Sound() = default;

		/// <param name="filepath">The path to a mono-channel wave file encoded with Float32</param>
		Sound(std::string filepath);

		Sound(const Sound&) = delete;
		Sound& operator=(const Sound&) = delete;

		/// @note Expected other to be unused
		Sound(Sound&&) noexcept;

		/// @note Expected other to be unused and this to be initialized by the default constructor
		Sound& operator=(Sound&&) noexcept;

		/// <summary>
		/// Play the sound from the position *emmitter
		/// </summary>
		/// <param name="emmitter">A pointer to the position that plays the sound</param>
		void play(const Vec2<int>* emmitter);

		/// @brief Stop all playing instances of the sound
		void stop();

		void terminate() override;

	private :

		static int s_audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		std::vector<std::unique_ptr<SoundData>> m_sounds_data;

		void m_stop_streams();
		std::thread m_thread;
		bool m_has_to_end = false;
	};

	/// <summary>
	/// A sound that can be played multiple times at once, it isn't related to a position in space.
	/// </summary>
	class HXG_DECLSPEC StaticSound : public SoundBase
	{
	public:

		StaticSound() = default;

		/// <param name="filepath">The path to a wave file encoded with Float32</param>
		StaticSound(std::string filepath);

		StaticSound(const StaticSound&) = delete;
		StaticSound& operator=(const StaticSound&) = delete;

		/// @note Expected other to be unused
		StaticSound(StaticSound&&) noexcept;

		/// @note Expected other to be unused and this to be initialized by the default constructor
		StaticSound& operator=(StaticSound&&) noexcept;

		/// @brief plays the sound
		void play();

		/// @brief stops all playing instances of the sound.
		void stop();

		void terminate() override;

	private:

		static int s_audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		std::vector<std::unique_ptr<SoundData>> m_sounds_data;

		void m_stop_streams();
		std::thread m_thread;
		bool m_has_to_end = false;
	};

	/// <summary>
	/// A Music that cannot be played multiple times at once, it isn't related to a position in space.
	/// </summary>
	class HXG_DECLSPEC Music : public SoundBase
	{
	public:

		Music() = default;

		/// <param name="filepath">The path to a wave file encoded with Float32</param>
		Music(std::string filepath);

		Music(const Music&) = delete;
		Music& operator=(const Music&) = delete;

		/// @note Expected other to be unused
		Music(Music&&) noexcept;

		/// @note Expected other to be unused and this to be initialized by the default constructor
		Music& operator=(Music&&) noexcept;

		/// @brief plays the music if it isn't already playing.
		void play();

		/// @brief stops the music.
		void stop();

		void terminate() override;

	private:

		static int s_audio_callback(const void* input_buffer, void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data);

		PaStream* m_stream = nullptr;
		float* m_buffer = nullptr;
		bool m_is_playing = false;
	};

}

#endif