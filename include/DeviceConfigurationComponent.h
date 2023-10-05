#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include "AudioRecorder.h"
#include "ComboBoxWithLabelComponent.h"

class DeviceConfigurationComponent : public juce::GroupComponent, public juce::ComboBox::Listener
{
public:
    DeviceConfigurationComponent(AudioRecorder &r);

    void resized() override;
    virtual void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;

private:
    void resetChannelButtons();

    ComboBoxWithLabelComponent deviceSelector;
    ComboBoxWithLabelComponent sampleRateSelector;
    ComboBoxWithLabelComponent bufferSizeSelector;
    AudioRecorder &audioRecorder;
    juce::ToggleButton channel1Button{"Channel 1"};
    juce::ToggleButton channel2Button{"Channel 2"};

    juce::String selectedDeviceName;
};