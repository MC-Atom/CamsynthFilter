# Camsynth Filter

This is a Juce plugin that works in conjunction with the [Camsynth Sender](https://github.com/MC-Atom/CamsynthSender) to control a filter based on the ratio of the amplitudes of high and low frequencies of an outline.

For a full demonstration and short walkthrough of how it works, check out my website: [https://devynoh.dev/portfolio/camsynth/](https://devynoh.dev/portfolio/camsynth/)

### Compiling and Running

This is a Juce application that doesn't have a compiled version on github. To compile this, download and compile through [Projucer](https://juce.com/download/). You may want to look at the [Juce getting started tutorial](https://juce.com/tutorials/tutorial_new_projucer_project/).
This will compile into VST and AU plugins that work in DAWs such as Logic Pro and Ableton.
This **NEEDS THE [CAMSYNTH SENDER](https://github.com/MC-Atom/CamsynthSender)** or it will just be a static filter.

### See also:
* [Camsynth Sender](https://github.com/MC-Atom/CamsynthSender): The computation-heavy main application that drives this application
* [Camsynth Player](https://github.com/MC-Atom/CamsynthPlayer): A similar application to this that uses an outline to generate sound using all of the partials of its fourier series.
