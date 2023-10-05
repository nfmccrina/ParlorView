#pragma once

#include <juce_audio_devices/juce_audio_devices.h>

class RecordedAudioProcessor : public juce::AudioIODeviceCallback
{
public:
    RecordedAudioProcessor() = default;
    RecordedAudioProcessor(const RecordedAudioProcessor &other) = delete;

    RecordedAudioProcessor &operator=(const RecordedAudioProcessor &other) = delete;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRecording() const = 0;

    virtual ~RecordedAudioProcessor()
    {
    }
};