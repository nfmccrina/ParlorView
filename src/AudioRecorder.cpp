#include "AudioRecorder.h"

AudioRecorder::AudioRecorder(std::unique_ptr<RecordedAudioProcessor> &&p)
    : processor(std::move(p)),
      currentDevice{nullptr},
      deviceType{nullptr},
      sampleRate{std::nullopt},
      bufferSize{std::nullopt}
{
    this->deviceType = std::unique_ptr<juce::AudioIODeviceType>(juce::AudioIODeviceType::createAudioIODeviceType_CoreAudio());
}

void AudioRecorder::startRecording()
{
    if (!this->sampleRate.has_value() || !this->bufferSize.has_value())
    {
        return;
    }

    juce::BigInteger activeChannels{0};

    for (std::vector<bool>::size_type channelIndex = 0; channelIndex < this->channels.size(); ++channelIndex)
    {
        if (this - channels[channelIndex])
        {
            activeChannels |= 1 << channelIndex;
        }
    }

    if (activeChannels == 0)
    {
        return;
    }

    juce::RuntimePermissions::request(
        juce::RuntimePermissions::recordAudio,
        [this, activeChannels](bool wasGranted)
        {
            if (wasGranted)
            {
                this->currentDevice->open(activeChannels, 0, this->sampleRate.value(), this->bufferSize.value());
                this->currentDevice->start(this->processor.get());
                this->processor->start();
            }
        });
}

void AudioRecorder::stopRecording()
{
    this->processor->stop();
    this->currentDevice->stop();
}

bool AudioRecorder::isRecording() const
{
    return this->processor->isRecording();
}

juce::StringArray AudioRecorder::getAvailableDeviceNames()
{
    if (this->deviceType == nullptr)
    {
        // not on Mac I guess
        return juce::StringArray{};
    }

    this->deviceType->scanForDevices();

    return this->deviceType->getDeviceNames(true);
}

void AudioRecorder::setAudioDevice(juce::String deviceName)
{
    if (this->currentDevice != nullptr)
    {
        if (this->currentDevice->isPlaying())
        {
            this->currentDevice->stop();
        }

        if (this->currentDevice->isOpen())
        {
            this->currentDevice->close();
        }

        this->currentDevice = nullptr;
    }

    if (this->deviceType == nullptr)
    {
        return;
    }

    this->currentDevice = std::unique_ptr<juce::AudioIODevice>(this->deviceType->createDevice("", deviceName));

    this->channels.clear();

    for (auto i = 0; i < this->currentDevice->getInputChannelNames().size(); ++i)
    {
        this->channels.push_back(false);
    }
}

juce::StringArray AudioRecorder::getChannelNames()
{
    if (this->currentDevice != nullptr)
    {
        return this->currentDevice->getInputChannelNames();
    }

    return juce::StringArray{};
}

void AudioRecorder::setChannelState(int index, bool isEnabled)
{
    this->channels[index] = isEnabled;
}

juce::StringArray AudioRecorder::getSampleRates()
{
    if (this->currentDevice == nullptr)
    {
        return juce::StringArray{};
    }

    auto sampleRates = this->currentDevice->getAvailableSampleRates();
    juce::StringArray results;

    for (auto availableSampleRate = sampleRates.begin(); availableSampleRate != sampleRates.end(); ++availableSampleRate)
    {
        results.add(juce::String{*availableSampleRate});
    }

    return results;
}

juce::StringArray AudioRecorder::getBufferSizes()
{
    if (this->currentDevice == nullptr)
    {
        return juce::StringArray{};
    }

    auto bufferSizes = this->currentDevice->getAvailableBufferSizes();
    juce::StringArray results;

    for (auto availableBufferSize = bufferSizes.begin(); availableBufferSize != bufferSizes.end(); ++availableBufferSize)
    {
        results.add(juce::String{*availableBufferSize});
    }

    return results;
}
void AudioRecorder::setSampleRate(std::optional<double> sr)
{
    this->sampleRate = sr;
}
void AudioRecorder::setBufferSize(std::optional<int> bs)
{
    this->bufferSize = bs;
}

AudioRecorder::~AudioRecorder()
{
    if (this->currentDevice != nullptr)
    {
        if (this->currentDevice->isPlaying())
        {
            this->currentDevice->stop();
        }

        if (this->currentDevice->isOpen())
        {
            this->currentDevice->close();
        }
    }
}