#include <juce_gui_basics/juce_gui_basics.h>
#include "FileWriterAudioProcessor.h"

void FileWriterAudioProcessor::audioDeviceIOCallbackWithContext(const float *const *inputChannelData, int numInputChannels, float *const *outputChannelData, int numOutputChannels, int numSamples, const juce::AudioIODeviceCallbackContext &context)
{
    juce::ignoreUnused(context);
    juce::ignoreUnused(numInputChannels);

    const juce::ScopedLock sl{this->writerLock};

    if (this->activeWriter.load() != nullptr)
    {
        this->activeWriter.load()->write(inputChannelData, numSamples);
    }

    for (auto i = 0; i < numOutputChannels; ++i)
    {
        if (outputChannelData[i] != nullptr)
        {
            juce::FloatVectorOperations::clear(outputChannelData[i], numSamples);
        }
    }
}

void FileWriterAudioProcessor::audioDeviceAboutToStart(juce::AudioIODevice *device)
{
    this->sampleRate = device->getCurrentSampleRate();
}

void FileWriterAudioProcessor::audioDeviceStopped()
{
    this->sampleRate = 0.0;
}

void FileWriterAudioProcessor::stop()
{
    {
        const juce::ScopedLock sl{this->writerLock};
        this->activeWriter = nullptr;
    }

    this->threadedWriter.reset();

    if (juce::FileInputStream inputStream(this->outputFile); inputStream.openedOk())
    {
        juce::File saveLocation = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getNonexistentChildFile("recording", ".wav");

        if (const auto outputStream = makeOutputStream(juce::URL(saveLocation)))
        {
            outputStream->writeFromInputStream(inputStream, -1);
        }
    }

    this->outputFile.deleteFile();
}

void FileWriterAudioProcessor::start()
{
    if (!juce::RuntimePermissions::isGranted(juce::RuntimePermissions::writeExternalStorage))
    {
        juce::RuntimePermissions::request(
            juce::RuntimePermissions::writeExternalStorage,
            [this](bool granted)
            {
                if (granted)
                {
                    this->start();
                }
            });

        return;
    }

    this->outputFile = this->outputDirectory.getNonexistentChildFile("parlorview", ".wav");
    this->stop();

    if (this->sampleRate > 0)
    {
        outputFile.deleteFile();

        if (auto fileStream = std::unique_ptr<juce::FileOutputStream>(outputFile.createOutputStream()))
        {
            juce::WavAudioFormat wavFormat;

            if (auto writer = wavFormat.createWriterFor(fileStream.get(), this->sampleRate, 1, 16, {}, 0))
            {
                fileStream.release();

                this->threadedWriter.reset(new juce::AudioFormatWriter::ThreadedWriter(writer, this->backgroundThread, 32768));

                const juce::ScopedLock sd(writerLock);
                this->activeWriter = this->threadedWriter.get();
            }
        }
    }
}

bool FileWriterAudioProcessor::isRecording() const
{
    return this->activeWriter.load() != nullptr;
}

FileWriterAudioProcessor::~FileWriterAudioProcessor()
{
    this->stop();
}

FileWriterAudioProcessor::FileWriterAudioProcessor()
{
#if (JUCE_ANDROID || JUCE_IOS)
    this->outputDirectory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory);
#else
    this->outputDirectory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);
#endif

#if (JUCE_MAC)
    this->outputDirectory = this->outputDirectory.getChildFile("Application Support");
#endif

    this->outputDirectory.getChildFile("ParlorView");

    if (!this->outputDirectory.exists())
    {
        this->outputDirectory.createDirectory();
    }

    this->backgroundThread.startThread();
}