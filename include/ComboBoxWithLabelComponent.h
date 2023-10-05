#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class ComboBoxWithLabelComponent : public juce::Component
{
public:
    ComboBoxWithLabelComponent();

    void setLabelText(juce::String &&labelText);
    void setComboBoxItems(juce::StringArray comboBoxOptions);
    void setComboBoxId(juce::String id);
    void clear();
    void resized() override;
    void addListener(juce::ComboBox::Listener &listener);

private:
    juce::ComboBox comboBox;
    juce::Label label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBoxWithLabelComponent)
};