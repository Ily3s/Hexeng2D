#include <fstream>
#include <chrono>
#include <thread>

#include "Audio.hpp"

namespace Hexeng::Audio
{

    PaError e;

    void init()
    {
        HXG_PA(Pa_Initialize());
    }

    void terminate()
    {
        for (Sound* sound : sounds)
            sound->terminate();
        HXG_PA(Pa_Terminate());
    }

    Sound::Sound(std::string filepath)
    {
        std::ifstream file{ filepath, std::ios::binary };

        file.seekg(0, std::ios::end);
        size_t file_lenght = file.tellg();
        file.seekg(0, std::ios::beg);

        m_file_buffer.insert(0, file_lenght, 0);
        file.read(&m_file_buffer[0], file_lenght);

        m_channels_nb = ((uint16_t*)&m_file_buffer[22])[0];
        m_sample_rate = ((uint32_t*)&m_file_buffer[24])[0];
        uint16_t bits_per_sample = ((uint16_t*)&m_file_buffer[34])[0];

        m_byte_per_sample = static_cast<size_t>(bits_per_sample / 8) * m_channels_nb;

        m_format;

        switch (bits_per_sample)
        {
        case 8 :
            m_format = paInt8;
            break;

        case 16 :
            m_format = paInt16;
            break;

        case 24 :
            m_format = paInt24;
            break;

        case 32 :
            m_format = paInt32;
            break;

        default :
            assert(false && "Unsupported format");
            break;
        }

        m_thread = std::thread([this]() {m_stop_streams(); });

        sounds.push_back(this);
    }

    // expected other to be unused
    Sound::Sound(Sound&& other) noexcept
        : m_file_buffer(std::move(other.m_file_buffer)),
        m_byte_per_sample(other.m_byte_per_sample), m_format(other.m_format),
        m_sample_rate(other.m_sample_rate), m_channels_nb(other.m_channels_nb)
    {
        other.terminate();
        m_thread = std::thread([this]() {m_stop_streams(); });

        auto it = std::find(sounds.begin(), sounds.end(), &other);
        *it = this;
    }

    // expected other to be unuesed and this to be uninitialized
    Sound& Sound::operator=(Sound&& other) noexcept
    {
        m_file_buffer = std::move(other.m_file_buffer);
        m_byte_per_sample = other.m_byte_per_sample;
        m_format = other.m_format;
        m_sample_rate = other.m_sample_rate;
        m_channels_nb = other.m_channels_nb;

        other.terminate();
        m_thread = std::thread([this]() {m_stop_streams(); });

        auto it = std::find(sounds.begin(), sounds.end(), &other);
        *it = this;

        return *this;
    }

    int Sound::audio_callback
        (const void* input_buffer, void* output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo* time_info,
        PaStreamCallbackFlags status_flags,
        void* user_data)
    {
        SoundData* data = (SoundData*)user_data;
        Sound* sound = data->sound;
        char* out = (char*)output_buffer;
        unsigned int i;
        
        for (i = 0; i < frames_per_buffer; i++)
        {
            if ((data->buffer >= &*(sound->m_file_buffer.end() - sound->m_byte_per_sample)) || data->has_to_stop)
            {
                data->is_playing = false;
                return paComplete;
            }
        
            memcpy(out, data->buffer, sound->m_byte_per_sample);
        
            data->buffer += sound->m_byte_per_sample;
            out += sound->m_byte_per_sample;
        }
        
        return 0;
    }

    void Sound::play()
    {
        m_sounds_data.push_back(std::make_unique<SoundData>(this, nullptr, &m_file_buffer[44], false, true));
        SoundData* data = m_sounds_data.back().get();

        HXG_PA(Pa_OpenDefaultStream(
            &data->stream,
            0,
            m_channels_nb,
            m_format,
            m_sample_rate,
            paFramesPerBufferUnspecified,
            audio_callback,
            data));

        HXG_PA(Pa_StartStream(data->stream));
    }

    void Sound::stop()
    {
        for (auto& sound_data : m_sounds_data)
            sound_data->has_to_stop = true;
    }

    void Sound::m_stop_streams()
    {
        while (m_has_to_end)
        {
            uint32_t i = 0;
            for (auto& sound_data : m_sounds_data)
            {
                if (sound_data->is_playing)
                    i++;
                else if (sound_data->stream)
                {
                    HXG_PA(Pa_StopStream(sound_data->stream));
                    HXG_PA(Pa_CloseStream(sound_data->stream));
                    sound_data->stream = nullptr;
                }
            }
            if (!i)
                m_sounds_data.clear();
        }
    }

    void Sound::terminate()
    {
        m_has_to_end = true;
        m_thread.join();
    }

}