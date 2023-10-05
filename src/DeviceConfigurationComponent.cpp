#include "DeviceConfigurationComponent.h"

void DeviceConfigurationComponent::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    auto selectedId = comboBoxThatHasChanged->getSelectedId();

    if (comboBoxThatHasChanged->getComponentID() == "deviceSelector")
    {
        this->selectedDeviceName = comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->indexOfItemId(selectedId));

        if (this->selectedDeviceName != "None")
        {
            this->audioRecorder.setAudioDevice(this->selectedDeviceName);
        }

        this->sampleRateSelector.clear();
        this->sampleRateSelector.setComboBoxItems(this->audioRecorder.getSampleRates());

        this->bufferSizeSelector.clear();
        this->bufferSizeSelector.setComboBoxItems(this->audioRecorder.getBufferSizes());

        this->resetChannelButtons();
    }

    if (comboBoxThatHasChanged->getComponentID() == "sampleRateSelector")
    {
        auto sampleRateText = comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->indexOfItemId(selectedId));

        if (sampleRateText != "None")
        {
            this->audioRecorder.setSampleRate(sampleRateText.getDoubleValue());
        }
        else
        {
            this->audioRecorder.setSampleRate(std::nullopt);
        }
    }

    if (comboBoxThatHasChanged->getComponentID() == "bufferSizeSelector")
    {
        auto bufferSizeText = comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->indexOfItemId(selectedId));

        if (bufferSizeText != "None")
        {
            this->audioRecorder.setBufferSize(bufferSizeText.getIntValue());
        }
        else
        {
            this->audioRecorder.setBufferSize(std::nullopt);
        }
    }
}

DeviceConfigurationComponent::DeviceConfigurationComponent(AudioRecorder &r)
    : audioRecorder{r}
{

    this->deviceSelector.setComboBoxId("deviceSelector");
    this->deviceSelector.setLabelText("Choose Audio Device:");
    this->deviceSelector.setComboBoxItems(this->audioRecorder.getAvailableDeviceNames());
    this->deviceSelector.addListener(*this);

    this->sampleRateSelector.setComboBoxId("sampleRateSelector");
    this->sampleRateSelector.setLabelText("Choose Sample Rate:");
    this->sampleRateSelector.setComboBoxItems(this->audioRecorder.getSampleRates());
    this->sampleRateSelector.addListener(*this);

    this->bufferSizeSelector.setComboBoxId("bufferSizeSelector");
    this->bufferSizeSelector.setLabelText("Choose Buffer Size:");
    this->bufferSizeSelector.setComboBoxItems(this->audioRecorder.getBufferSizes());
    this->bufferSizeSelector.addListener(*this);

    this->setText("Audio Configuration");

    this->channel1Button.setEnabled(false);
    this->channel2Button.setEnabled(false);

    addAndMakeVisible(this->deviceSelector);
    addAndMakeVisible(this->sampleRateSelector);
    addAndMakeVisible(this->bufferSizeSelector);
    addAndMakeVisible(this->channel1Button);
    addAndMakeVisible(this->channel2Button);

    // auto availableChannels = this->audioDeviceChannelFinder.getChannelNames()
}

void DeviceConfigurationComponent::resetChannelButtons()
{
    auto channelNames = this->audioRecorder.getChannelNames();

    this->channel1Button.setEnabled(true);
    this->channel2Button.setEnabled(true);

    if (channelNames.size() < 1)
    {
        this->channel1Button.setEnabled(false);
        this->channel2Button.setEnabled(false);
    }

    if (channelNames.size() < 2)
    {
        this->channel2Button.setEnabled(false);
    }

    this->resized();
}

void DeviceConfigurationComponent::resized()
{
    juce::FlexBox flexBox;

    flexBox.flexDirection = juce::FlexBox::Direction::column;

    flexBox.items.add(juce::FlexItem(this->deviceSelector).withFlex(0.2f));
    flexBox.items.add(juce::FlexItem(this->sampleRateSelector).withFlex(0.2f));
    flexBox.items.add(juce::FlexItem(this->bufferSizeSelector).withFlex(0.2f));
    flexBox.items.add(juce::FlexItem(this->channel1Button).withFlex(0.1f).withMinWidth(100).withAlignSelf(juce::FlexItem::AlignSelf::flexEnd));
    flexBox.items.add(juce::FlexItem(this->channel2Button).withFlex(0.1f).withMinWidth(100).withAlignSelf(juce::FlexItem::AlignSelf::flexEnd));

    flexBox.performLayout(getLocalBounds());
}