#ifndef XNA_FRAMEWORK_AUDIO_WAVFILE_HPP
#define XNA_FRAMEWORK_AUDIO_WAVFILE_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include "Xna/CSharp/BitConverter.hpp"
#include "Xna/CSharp/IO/Stream.hpp"
#include "AudioFormat.hpp"
#include "Xna/CSharp/IO/BinaryReader.hpp"
#include "LoopRegion.hpp"

namespace Xna {
    class WavFile final {
    public:
        WavFile(std::shared_ptr<CSharp::Stream> const& source);

        static WavFile Open(std::shared_ptr<CSharp::Stream> const& stream) {
            return WavFile(stream);
        }

    private:
        void ParseWavHeader();
        void ReadChunk();
        void ParseFormat();
        void ParseData();
        void ParseLoopRegion();
        void ParseWsmpChunk();
        void ParseSmplChunk();

        //Internal
    public:
        constexpr std::optional<AudioFormat> Format() const { return format; }
        constexpr std::optional<std::vector<uint8_t>> Data() const { return buffer; }
        constexpr int32_t LoopStart() const { return loop.Start; }
        constexpr int32_t LoopLength() const { return loop.Length; }
        constexpr int32_t SampleLength() const { return buffer->size() / format->BlockAlign; }
        constexpr CSharp::TimeSpan Duration() const { return format->DurationFromSize(buffer->size()); }

    private:
        struct RiffChunk {
            int32_t id{};
            std::vector<uint8_t> data;
            RiffChunk() = default;

            RiffChunk(int32_t id, const std::vector<uint8_t>& data) : id(id), data(data) {
            }
        };

        std::optional<CSharp::BinaryReader> reader;
        std::optional<AudioFormat> format;
        std::optional<std::vector<uint8_t>> buffer;
        std::optional<RiffChunk> smplChunk;
        std::optional<RiffChunk> wsmpChunk;
        LoopRegion loop{};

        static constexpr uint8_t riff[] = {
            (uint8_t)82, (uint8_t)73, 
            (uint8_t)70, (uint8_t)70 
        };

        static constexpr uint8_t wave[] = { 
            (uint8_t)87, (uint8_t)65,
            (uint8_t)86, (uint8_t)69 
        };

        static constexpr uint8_t fmt[] = { 
            (uint8_t)102, (uint8_t)109, (uint8_t)116, (uint8_t)32 
        };
        
        static constexpr uint8_t data[] = {
            (uint8_t)100, (uint8_t)97,
            (uint8_t)116, (uint8_t)97 
        };

        static constexpr uint8_t smpl[] = { 
            (uint8_t)115, (uint8_t)109, 
            (uint8_t)112, (uint8_t)108 
        };

        static constexpr uint8_t wsmp[] = { 
            (uint8_t)119, (uint8_t)115, 
            (uint8_t)109, (uint8_t)112 
        };

        static inline int32_t riffId = CSharp::BitConverter::ToInt32(WavFile::riff, 4, 0);
        static inline int32_t wavId = CSharp::BitConverter::ToInt32(WavFile::wave, 4, 0);
        static inline int32_t fmtId = CSharp::BitConverter::ToInt32(WavFile::fmt, 4, 0);
        static inline int32_t smplId = CSharp::BitConverter::ToInt32(WavFile::smpl, 4, 0);
        static inline int32_t wsmpId = CSharp::BitConverter::ToInt32(WavFile::wsmp, 4, 0);
        static inline int32_t dataId = CSharp::BitConverter::ToInt32(WavFile::data, 4, 0);
    };
}

#endif //XNA_WAVFILE_HPP