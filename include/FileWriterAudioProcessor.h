#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "RecordedAudioProcessor.h"

class FileWriterAudioProcessor : public RecordedAudioProcessor
{
public:
    void audioDeviceIOCallbackWithContext(const float *const *inputChannelData, int numInputChannels, float *const *outputChannelData, int numOutputChannels, int numSamples, const juce::AudioIODeviceCallbackContext &context) override;
    void audioDeviceAboutToStart(juce::AudioIODevice *device) override;
    void audioDeviceStopped() override;
    void stop() override;
    void start() override;
    bool isRecording() const override;

    FileWriterAudioProcessor();

    ~FileWriterAudioProcessor() override;

private:
    juce::TimeSliceThread backgroundThread{"Audio Recorder Thread"};
    std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> threadedWriter;
    juce::CriticalSection writerLock;
    std::atomic<juce::AudioFormatWriter::ThreadedWriter *> activeWriter{nullptr};

    double sampleRate = 0.0;
    juce::File outputDirectory;
    juce::File outputFile;
};

inline std::unique_ptr<juce::OutputStream> makeOutputStream(const juce::URL &url)
{
    if (const auto doc = juce::AndroidDocument::fromDocument(url))
        return doc.createOutputStream();

#if !JUCE_IOS
    if (url.isLocalFile())
        return url.getLocalFile().createOutputStream();
#endif

    return url.createOutputStream();
}