#include "ComboBoxWithLabelComponent.h"

ComboBoxWithLabelComponent::ComboBoxWithLabelComponent()
    : label{"", ""}
{
    addAndMakeVisible(this->label);
    addAndMakeVisible(this->comboBox);
}

void ComboBoxWithLabelComponent::setLabelText(juce::String &&labelText)
{
    this->label.setText(labelText, juce::NotificationType::dontSendNotification);
}

void ComboBoxWithLabelComponent::setComboBoxItems(juce::StringArray comboBoxOptions)
{
    this->comboBox.clear(juce::NotificationType::dontSendNotification);
    this->comboBox.addItemList(comboBoxOptions, 1);
    this->comboBox.addItem(juce::translate("None"), -1);
    this->comboBox.setSelectedId(-1, juce::NotificationType::dontSendNotification);
}

void ComboBoxWithLabelComponent::resized()
{
    juce::FlexBox flexBox;

    flexBox.flexDirection = juce::FlexBox::Direction::row;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    flexBox.items.add(juce::FlexItem(this->label).withFlex(0.1f).withMaxHeight(20).withMargin(juce::FlexItem::Margin{0, 0, 0, 20}));
    flexBox.items.add(juce::FlexItem(this->comboBox).withFlex(0.1f).withMaxHeight(20).withMargin(juce::FlexItem::Margin{0, 20, 0, 0}));

    flexBox.performLayout(getLocalBounds().withTrimmedTop(30));
}

void ComboBoxWithLabelComponent::addListener(juce::ComboBox::Listener &listener)
{
    this->comboBox.addListener(&listener);
}

void ComboBoxWithLabelComponent::clear()
{
    this->comboBox.clear(juce::NotificationType::dontSendNotification);
    this->comboBox.addItem(juce::translate("None"), -1);
    this->comboBox.setSelectedId(-1, juce::NotificationType::dontSendNotification);
}

void ComboBoxWithLabelComponent::setComboBoxId(juce::String id)
{
    this->comboBox.setComponentID(id);
}