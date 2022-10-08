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

    const Vec2<int>* receiver = Renderer::Camera::xy_position;

    std::vector<SoundBase*> sounds;

    void init()
    {
        HXG_PA(Pa_Initialize());
    }

    SoundBase::SoundBase(std::string filepath)
    {
        HXG_ASSERT(filepath.size() > 4 && (std::string{ filepath.end() - 4, filepath.end() } == ".wav"),
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

        other.terminate();

        auto it = std::find(sounds.begin(), sounds.end(), &other);
        *it = (SoundBase*)this;

        return *this;
    }

    Sound::Sound(std::string filepath)
        : SoundBase(filepath)
    {
        HXG_ASSERT(m_channels_nb == 1,
            HXG_LOG_ERROR("Input for Sound is expected to be mono"); return;);
        m_thread = std::thread([this]() {m_stop_streams(); });
    }

    Sound::Sound(Sound&& other) noexcept
        : SoundBase(std::move(other))
    {
        m_thread = std::thread([this]() {m_stop_streams(); });
    }

    Sound& Sound::operator=(Sound&& other) noexcept
    {
        SoundBase::operator=(std::move(other));
        m_thread = std::thread([this]() {m_stop_streams(); });
        return *this;
    }

    void Sound::play(const Vec2<int>* emmitter)
    {
        m_garbage.push_back(std::thread([this, emmitter]() {
            m_sounds_data.push_back(std::make_unique<SoundData>((void*)this, nullptr, (float*)&m_file_buffer[44], emmitter));
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
            }));
    }

    void Sound::stop()
    {
        for (auto& sound_data : m_sounds_data)
            sound_data->has_to_stop = true;
    }

    void Sound::terminate()
    {
        stop();
        m_has_to_end = true;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
        m_thread.join();
    }

    int Sound::s_audio_callback
    (const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        SoundData* data = (SoundData*)user_data;
        Sound* sound = (Sound*)data->sound;
        float* out = (float*)output_buffer;

        float volume_multiplier = 1.0f;
        float right_multiplier = 1.0f;
        float left_multiplier = 1.0f;

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
                data->is_playing = false;
                return paComplete;
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

    void Sound::m_stop_streams()
    {
        while (!m_has_to_end)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            for (size_t i = 0; i < m_sounds_data.size(); i++)
            {
                if (!m_sounds_data[i]->is_playing)
                {
                    HXG_PA(Pa_StopStream(m_sounds_data[i]->stream));
                    HXG_PA(Pa_CloseStream(m_sounds_data[i]->stream));
                    m_sounds_data.erase(m_sounds_data.begin() + i);
                }
            }
        }
    }

    StaticSound::StaticSound(std::string filepath)
        : SoundBase(filepath)
    {
        m_thread = std::thread([this]() {m_stop_streams(); });
    }

    StaticSound::StaticSound(StaticSound&& other) noexcept
        : SoundBase(std::move(other))
    {
        m_thread = std::thread([this]() {m_stop_streams(); });
    }

    StaticSound& StaticSound::operator=(StaticSound&& other) noexcept
    {
        SoundBase::operator=(std::move(other));
        m_thread = std::thread([this]() {m_stop_streams(); });
        return *this;
    }

    void StaticSound::play()
    {
        m_garbage.push_back(std::thread([this]() {
            m_sounds_data.push_back(std::make_unique<SoundData>((void*)this, nullptr, (float*)&m_file_buffer[44], nullptr));
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
            }));
    }

    void StaticSound::stop()
    {
        for (auto& sound_data : m_sounds_data)
            sound_data->has_to_stop = true;
    }

    void StaticSound::terminate()
    {
        stop();
        m_has_to_end = true;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
        m_thread.join();
    }

    int StaticSound::s_audio_callback(const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        SoundData* data = (SoundData*)user_data;
        StaticSound* sound = (StaticSound*)data->sound;
        float* out = (float*)output_buffer;

        for (uint32_t i = 0; i < frames_per_buffer * sound->m_channels_nb; i++, data->buffer++, out++)
        {
            if ((char*)data->buffer > &sound->m_file_buffer[sound->m_file_buffer.size() - 1] || data->has_to_stop)
            {
                data->is_playing = false;
                return paComplete;
            }

            *out = *data->buffer * Settings::master_volume * Settings::sound_volume;
        }

        return 0;
    }

    void StaticSound::m_stop_streams()
    {
        while (!m_has_to_end)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            for (size_t i = 0; i < m_sounds_data.size(); i++)
            {
                if (!m_sounds_data[i]->is_playing)
                {
                    HXG_PA(Pa_StopStream(m_sounds_data[i]->stream));
                    HXG_PA(Pa_CloseStream(m_sounds_data[i]->stream));
                    m_sounds_data.erase(m_sounds_data.begin() + i);
                }
            }
        }
    }

    Music::Music(std::string filepath)
        : SoundBase(filepath)
    {}

    Music::Music(Music&& other) noexcept
        : SoundBase(std::move(other))
    {}

    Music& Music::operator=(Music&& other) noexcept
    {
        SoundBase::operator=(std::move(other));
        return *this;
    }

    void Music::play()
    {
        m_garbage.push_back(std::thread([this]() {
            if (m_is_playing)
                return;

            if (m_stream)
                stop();

            m_buffer = (float*)&m_file_buffer[44];

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
            }));
    }

    void Music::stop()
    {
        m_garbage.push_back(std::thread([this]() {
            if (m_stream)
            {
                HXG_PA(Pa_StopStream(m_stream));
                HXG_PA(Pa_CloseStream(m_stream));
                m_stream = nullptr;
                m_is_playing = false;
            }
            }));
    }

    void Music::terminate()
    {
        stop();
    }

    int Music::s_audio_callback(const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        Music* music = (Music*)user_data;
        float* out = (float*)output_buffer;

        for (uint32_t i = 0; i < frames_per_buffer * music->m_channels_nb; i++, music->m_buffer++, out++)
        {
            if ((char*)music->m_buffer > &music->m_file_buffer[music->m_file_buffer.size() - 1])
            {
                music->m_is_playing = false;
                return paComplete;
            }

            *out = *music->m_buffer * Settings::master_volume * Settings::music_volume;
        }

        return 0;
    }

    bool has_to_end = false;

    void clean_garbages()
    {
        while (!has_to_end)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            for (auto sound : sounds)
            {
                for (auto& thread : sound->m_garbage)
                    thread.join();
                sound->m_garbage.clear();
            }
        }
    }

    std::thread clean_garbages_thread{ clean_garbages };

    void terminate()
    {
        for (SoundBase* sound : sounds)
            sound->terminate();
        has_to_end = true;
        clean_garbages_thread.join();
        HXG_PA(Pa_Terminate());
    }

}