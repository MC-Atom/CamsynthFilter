/*
  ==============================================================================

    SocketServertemp.h
    Created: 18 Jul 2025 9:16:59pm
    Author:  Devyn Oh

  ==============================================================================
*/

#pragma once
#include <atomic>
#include <cstdio>
#include <memory>

#include <JuceHeader.h>

#define MAX_URL_SIZE 20000

class SocketServer final : public juce::Thread {
public:
  std::atomic<int>*cutFreq;
  juce::Label *httpState;

  SocketServer (std::atomic<int>* cutFreq, juce::Label *httpState_input) : Thread("Filter Socket Server") {


    httpState = httpState_input;
    startThread();
  }

  ~SocketServer() override
  {
    // allow the thread 1 second to stop cleanly - should be plenty of time.
    stopThread (1000);
  }

  void run() override;

  std::function<void()> onSocketStatusChange;

private:
  static juce::AudioBuffer<float> processJson(char* body, int wavetableSize);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SocketServer);
};
