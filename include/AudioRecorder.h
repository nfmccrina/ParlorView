#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <juce_audio_devices/juce_audio_devices.h>
#include "RecordedAudioProcessor.h"

class AudioRecorder
{
public:
    void startRecording();
    void stopRecording();
    bool isRecording() const;
    juce::StringArray getAvailableDeviceNames();
    void setAudioDevice(juce::String deviceName);
    void setChannelState(int index, bool isEnabled);
    juce::StringArray getChannelNames();
    juce::StringArray getSampleRates();
    juce::StringArray getBufferSizes();
    void setSampleRate(std::optional<double> sr);
    void setBufferSize(std::optional<int> bs);

    AudioRecorder(std::unique_ptr<RecordedAudioProcessor> &&p);
    AudioRecorder(const AudioRecorder &other) = delete;
    AudioRecorder &operator=(const AudioRecorder &other) = delete;
    ~AudioRecorder();

private:
    std::unique_ptr<RecordedAudioProcessor> processor;
    juce::AudioDeviceManager audioDeviceManager;
    std::unique_ptr<juce::AudioIODevice> currentDevice;
    std::unique_ptr<juce::AudioIODeviceType> deviceType;
    std::vector<bool> channels;
    std::optional<double> sampleRate;
    std::optional<int> bufferSize;
};