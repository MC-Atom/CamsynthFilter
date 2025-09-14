/*
==============================================================================

    SocketServer.h
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
    std::atomic<int>*bias;
    std::atomic<int>*midpoint;
    std::atomic<float>*filterScalar;
    juce::Label *httpState;

    SocketServer (std::atomic<int>* cutFreq_input, std::atomic<int>* bias_input, std::atomic<int>* midpoint_input, std::atomic<float>* scalar_input, juce::Label *httpState_input) : Thread("Filter Socket Server") {

        bias = bias_input;
        cutFreq = cutFreq_input;
        midpoint = midpoint_input;
        filterScalar = scalar_input;
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
    static int processJson(char* body, int bias, int midpoint, float scalar);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SocketServer);
};
