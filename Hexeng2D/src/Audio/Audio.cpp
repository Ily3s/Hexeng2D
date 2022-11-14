#include <fstream>
#include <chrono>
#include <thread>

#include "Audio.hpp"
#include "../Renderer/Camera.hpp"
#include "../Hexeng.hpp"

namespace Hexeng::Audio
{

    PaError e;

#if HXG_DEBUG_LEVEL >= 2

#define HXG_PA(x) \
e = x;\
if(e != paNoError)\
{std::cout << Pa_GetErrorText(e) << std::endl;\
 assert(e == paNoError);}""

#else

#define HXG_PA(x) x

#endif

    std::vector<SoundBase*> sounds;

    void init()
    {
        HXG_PA(Pa_Initialize());
    }

    void terminate()
    {
        for (SoundBase* sound : sounds)
            sound->terminate();
        HXG_PA(Pa_Terminate());
    }

    SoundBase::SoundBase(std::string filepath)
    {
        HXG_ASSERT(((filepath.size() > 4) && (std::string{ filepath.end() - 4, filepath.end() } == ".wav")),
            HXG_LOG_ERROR("The File \"" + filepath + "\" is not a wave file. It is supposed to be one."); return;);

        std::ifstream file{ filepath, std::ios::binary };

        HXG_ASSERT(file,
            HXG_LOG_ERROR("The file \"" + filepath + "\" may not exists"); return;);

        file.seekg(0, std::ios::end);
        size_t file_lenght = file.tellg();
        file.seekg(0, std::ios::beg);

        HXG_ASSERT(file_lenght > 44,
            HXG_LOG_ERROR("Unvalid Audio file \"" + filepath + "\""); return;);

        m_file_buffer.insert(0, file_lenght, 0);
        file.read(&m_file_buffer[0], file_lenght);

        m_channels_nb = ((uint16_t*)&m_file_buffer[22])[0];
        m_sample_rate = ((uint32_t*)&m_file_buffer[24])[0];
        uint16_t bits_per_sample = ((uint16_t*)&m_file_buffer[34])[0];

        m_byte_per_sample = static_cast<size_t>(bits_per_sample / 8) * m_channels_nb;

        HXG_ASSERT(bits_per_sample == 32,
            HXG_LOG_ERROR("Audio Systsem only supports Float32 sample format"); return;);

        sounds.push_back(this);
    }

    SoundBase::SoundBase(SoundBase&& other) noexcept
        : m_file_buffer(std::move(other.m_file_buffer)),
        m_byte_per_sample(other.m_byte_per_sample),
        m_sample_rate(other.m_sample_rate),
        m_channels_nb(other.m_channels_nb)
    {
        other.terminate();

        auto it = std::find(sounds.begin(), sounds.end(), &other);
        *it = (SoundBase*)this;
    }

    SoundBase& SoundBase::operator=(SoundBase&& other) noexcept
    {
        m_file_buffer = std::move(other.m_file_buffer);
        m_byte_per_sample = other.m_byte_per_sample;
        m_sample_rate = other.m_sample_rate;
        m_channels_nb = other.m_channels_nb;

        auto it = std::find(sounds.begin(), sounds.end(), &other);
        *it = (SoundBase*)this;

        return *this;
    }

    Music::Music(std::string filepath)
        : SoundBase(filepath)
    {
        HXG_PA(Pa_OpenDefaultStream(
            &m_stream,
            0,
            m_channels_nb,
            paFloat32,
            m_sample_rate,
            paFramesPerBufferUnspecified,
            s_audio_callback,
            this));

        HXG_PA(Pa_StartStream(m_stream));
    }

    Music::Music(Music&& other) noexcept
        : SoundBase(std::move(other)), m_stream(other.m_stream)
    {
        HXG_PA(Pa_StopStream(m_stream));
        HXG_PA(Pa_CloseStream(m_stream));

        HXG_PA(Pa_OpenDefaultStream(
            &m_stream,
            0,
            m_channels_nb,
            paFloat32,
            m_sample_rate,
            paFramesPerBufferUnspecified,
            s_audio_callback,
            this));

        HXG_PA(Pa_StartStream(m_stream));
    }

    Music& Music::operator=(Music&& other) noexcept
    {
        SoundBase::operator=(std::move(other));
        m_stream = other.m_stream;

        HXG_PA(Pa_StopStream(m_stream));
        HXG_PA(Pa_CloseStream(m_stream));

        HXG_PA(Pa_OpenDefaultStream(
            &m_stream,
            0,
            m_channels_nb,
            paFloat32,
            m_sample_rate,
            paFramesPerBufferUnspecified,
            s_audio_callback,
            this));

        HXG_PA(Pa_StartStream(m_stream));

        return *this;
    }

    void Music::play()
    {
        m_buffer = (float*)&(m_file_buffer[44 * 4]);
    }

    void Music::stop()
    {
        m_buffer = nullptr;
    }

    void Music::terminate()
    {
        HXG_PA(Pa_StopStream(m_stream));
        HXG_PA(Pa_CloseStream(m_stream));
    }

    int Music::s_audio_callback(const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        Music* music = (Music*)user_data;
        float* out = (float*)output_buffer;

        if (!(music->m_buffer))
        {
            for (uint32_t i = 0; i < frames_per_buffer * music->m_channels_nb; i++, out++)
                *out = 0;

            return 0;
        }

        for (uint32_t i = 0; i < frames_per_buffer * music->m_channels_nb; i++, music->m_buffer++, out++)
        {
            if ((char*)music->m_buffer > &music->m_file_buffer[music->m_file_buffer.size() - 1])
            {
                music->m_buffer = nullptr;
                return 0;
            }

            *out = *music->m_buffer * Settings::master_volume * Settings::music_volume;
        }

        return 0;
    }

    const Vec2<int>* receiver = Renderer::Camera::xy_position;

    void Sound::reserve(size_t count)
    {
        if (!m_has_done_reserving)
            return;

        m_has_done_reserving = false;
        count = count < m_sounds_data.size() ? 0 : count - m_sounds_data.size();

        if (m_reserve_thread.joinable())
            m_reserve_thread.join();

        m_reserve_thread = std::thread{ [this, count]()
        {
            for (size_t i = 0; i < count; i++)
            {
                m_sounds_data.push_back(std::make_unique<SoundData>((void*)this, nullptr, nullptr, nullptr));
                SoundData* data = m_sounds_data.back().get();

                HXG_PA(Pa_OpenDefaultStream(
                    &data->stream,
                    0,
                    m_channels_nb,
                    paFloat32,
                    m_sample_rate,
                    paFramesPerBufferUnspecified,
                    s_audio_callback,
                    data));

                HXG_PA(Pa_StartStream(data->stream));
            }
            m_has_done_reserving = true;
        } };
    }

    Sound::Sound(std::string filepath)
        : SoundBase(filepath)
    {
        for (int i = 0; i < 4; i++)
        {
            m_sounds_data.push_back(std::make_unique<SoundData>((void*)this, nullptr, nullptr, nullptr));
            SoundData* data = m_sounds_data.back().get();

            HXG_PA(Pa_OpenDefaultStream(
                &data->stream,
                0,
                m_channels_nb,
                paFloat32,
                m_sample_rate,
                paFramesPerBufferUnspecified,
                s_audio_callback,
                data));

            HXG_PA(Pa_StartStream(data->stream));
        }
    }

    Sound::Sound(Sound&& other) noexcept
        : SoundBase(std::move(other)), m_sounds_data(std::move(other.m_sounds_data))
    {
        for (auto& data : m_sounds_data)
            data->sound = (void*)this;
    }

    Sound& Sound::operator=(Sound&& other) noexcept
    {
        SoundBase::operator=(std::move(other));
        m_sounds_data = std::move(other.m_sounds_data);

        for (auto& data : m_sounds_data)
            data->sound = (void*)this;

        return *this;
    }

    void Sound::play()
    {
        auto it = std::find_if(m_sounds_data.begin(), m_sounds_data.end(),
            [](const std::unique_ptr<SoundData>& data) {return !(data->buffer); });

        if (it != m_sounds_data.end())
            (*it)->buffer = (float*)&(m_file_buffer[44 * 4]);
        else
            reserve(m_sounds_data.size() + 4);
    }

    void Sound::stop()
    {
        for (auto& sound_data : m_sounds_data)
            sound_data->has_to_stop = true;
    }

    void Sound::terminate()
    {
        if (m_reserve_thread.joinable())
            m_reserve_thread.join();

        for (auto& sound_data : m_sounds_data)
        {
            HXG_PA(Pa_AbortStream(sound_data->stream));
            HXG_PA(Pa_CloseStream(sound_data->stream));
        }
    }

    int Sound::s_audio_callback(const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        SoundData* data = (SoundData*)user_data;
        Sound* sound = (Sound*)data->sound;
        float* out = (float*)output_buffer;

        if (!data->buffer)
        {
            for (uint32_t i = 0; i < frames_per_buffer * sound->m_channels_nb; i++, out++)
                *out = 0;

            return 0;
        }

        for (uint32_t i = 0; i < frames_per_buffer * sound->m_channels_nb; i++, data->buffer++, out++)
        {
            if ((char*)data->buffer > &sound->m_file_buffer[sound->m_file_buffer.size() - 1] || data->has_to_stop)
            {
                data->buffer = nullptr;
                return 0;
            }

            *out = *data->buffer * Settings::master_volume * Settings::sound_volume;
        }

        return 0;
    }

    SpatialSound::SpatialSound(std::string filepath)
        : Sound(filepath)
    {
        HXG_ASSERT(m_channels_nb == 1,
            HXG_LOG_ERROR("Input for Sound is expected to be mono"); return;);

        terminate();
        m_sounds_data.clear();

        for (size_t i = 0; i < 4; i++)
        {
            m_sounds_data.push_back(std::make_unique<SoundData>((void*)this, nullptr, nullptr, nullptr));
            SoundData* data = m_sounds_data.back().get();

            HXG_PA(Pa_OpenDefaultStream(
                &data->stream,
                0,
                2,
                paFloat32,
                m_sample_rate,
                paFramesPerBufferUnspecified,
                s_audio_callback,
                data));

            HXG_PA(Pa_StartStream(data->stream));
        }
    }

    SpatialSound::SpatialSound(SpatialSound&& other) noexcept
        : Sound(std::move(other)) {}

    SpatialSound& SpatialSound::operator=(SpatialSound&& other) noexcept
    {
        Sound::operator=(std::move(other));
        return *this;
    }

    void SpatialSound::reserve(size_t count)
    {
        if (!m_has_done_reserving)
            return;

        m_has_done_reserving = false;
        count = count < m_sounds_data.size() ? 0 : count - m_sounds_data.size();

        if (m_reserve_thread.joinable())
            m_reserve_thread.join();

        m_reserve_thread = std::thread{ [this, count]()
        {
            for (size_t i = 0; i < count; i++)
            {
                m_sounds_data.push_back(std::make_unique<SoundData>((void*)this, nullptr, nullptr, nullptr));
                SoundData* data = m_sounds_data.back().get();

                HXG_PA(Pa_OpenDefaultStream(
                    &data->stream,
                    0,
                    2,
                    paFloat32,
                    m_sample_rate,
                    paFramesPerBufferUnspecified,
                    s_audio_callback,
                    data));

                HXG_PA(Pa_StartStream(data->stream));
            }
            m_has_done_reserving = true;
        } };
    }

    void SpatialSound::play(const Vec2<int>* emmitter)
    {
        auto it = std::find_if(m_sounds_data.begin(), m_sounds_data.end(),
            [](const std::unique_ptr<SoundData>& data) {return !(data->buffer); });

        if (it != m_sounds_data.end())
        {
            (*it)->buffer = (float*)&(m_file_buffer[44 * 4]);
            (*it)->emitter = emmitter;
        }
        else
        {
            reserve(m_sounds_data.size() + 4);
        }
    }

    int SpatialSound::s_audio_callback
    (const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        SoundData* data = (SoundData*)user_data;
        SpatialSound* sound = (SpatialSound*)data->sound;
        float* out = (float*)output_buffer;

        float volume_multiplier = 1.0f;
        float right_multiplier = 1.0f;
        float left_multiplier = 1.0f;

        if (!data->buffer)
        {
            for (uint32_t i = 0; i < frames_per_buffer * 2; i++, out++)
                *out = 0;

            return 0;
        }

        if (data->emitter && receiver)
        {
            uint32_t dist = std::max(Vec2<int>{ *data->emitter - *receiver }.len(), 100);
            int32_t x_diff = data->emitter->x - receiver->x;
            int32_t y_dist = std::abs(data->emitter->y - receiver->y);
            uint32_t x_dist = std::max(std::abs(x_diff), y_dist);
            if (x_diff > 0)
                left_multiplier = 1.0f / (static_cast<float>(x_dist) / y_dist);
            else
                right_multiplier = 1.0f / (static_cast<float>(x_dist) / y_dist);
            volume_multiplier = 1.0f / (static_cast<float>(dist) / 50);
        }

        for (uint32_t i = 0; i < frames_per_buffer; i++)
        {
            if (((char*)data->buffer >= &*(sound->m_file_buffer.end() - sound->m_byte_per_sample)) || data->has_to_stop)
            {
                data->buffer = nullptr;
                return 0;
            }

            // Left

            *out = *(float*)data->buffer * volume_multiplier * left_multiplier * Settings::master_volume * Settings::sound_volume;
            out++;

            // Right

            *out = *(float*)data->buffer * volume_multiplier * right_multiplier * Settings::master_volume * Settings::sound_volume;
            out++;

            data->buffer++;
        }

        return 0;
    }

}