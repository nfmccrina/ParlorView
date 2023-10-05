#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "AudioRecorder.h"
#include "DeviceConfigurationComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component
{
public:
    //==============================================================================
    MainComponent();

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    ~MainComponent();

private:
    //==============================================================================
    // Your private member variables go here...
    AudioRecorder audioRecorder;
    juce::TextButton recordButton{"Start Recording"};
    bool areLabelsVisible;
    juce::OwnedArray<juce::Label> labels;
    DeviceConfigurationComponent deviceConfigurationComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};