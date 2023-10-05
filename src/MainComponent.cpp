#include <memory>
#include <juce_core/juce_core.h>
#include "MainComponent.h"
#include "FileWriterAudioProcessor.h"

//==============================================================================
MainComponent::MainComponent()
    : audioRecorder{std::make_unique<FileWriterAudioProcessor>()},
      areLabelsVisible{false},
      deviceConfigurationComponent{audioRecorder}
{
    setSize(600, 400);
    setOpaque(true);

    addAndMakeVisible(this->deviceConfigurationComponent);
    addAndMakeVisible(this->recordButton);

    this->recordButton.onClick = [this]
    {
        if (!this->audioRecorder.isRecording())
        {
            this->recordButton.setButtonText("Stop Recording");
            this->audioRecorder.startRecording();
        }
        else
        {
            this->recordButton.setButtonText("Start Recording");
            this->audioRecorder.stopRecording();
        }
    };
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // g.setFont(juce::Font(16.0f));
    // g.setColour(juce::Colours::white);
    // g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    juce::FlexBox flexBox;

    flexBox.flexDirection = juce::FlexBox::Direction::column;
    flexBox.alignItems = juce::FlexBox::AlignItems::center;

    flexBox.items.add(juce::FlexItem(this->deviceConfigurationComponent).withFlex(0.8f).withAlignSelf(juce::FlexItem::AlignSelf::stretch));
    flexBox.items.add(juce::FlexItem(this->recordButton).withFlex(0.2f).withMaxHeight(20).withMaxWidth(100).withMinWidth(100).withMargin(juce::FlexItem::Margin{10, 0, 10, 0}).withAlignSelf(juce::FlexItem::AlignSelf::center));
    flexBox.performLayout(getLocalBounds());
}

MainComponent::~MainComponent()
{
}