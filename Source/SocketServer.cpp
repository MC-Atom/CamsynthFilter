/*
  ==============================================================================

    SocketServer.cpp
    Created: 18 Jul 2025 9:16:59pm
    Author:  Devyn Oh

  ==============================================================================
*/

// lsof -i :8080

#include "SocketServer.h"

#include <fstream>

#define InitPortNumber 8080
#define localHostName "0.0.0.0"

void SocketServer::run()
{
  std::printf("here\n");
  while (!onSocketStatusChange) { // Make sure onSocketStatusChange is defined before continuing
    sleep(10);
  }

  const auto socket = std::make_unique<juce::StreamingSocket>();
  int portNumber = InitPortNumber;
  bool listening = socket->createListener(portNumber, localHostName);
  std::printf("here\n");
  while (!listening) {
    portNumber += 1;
    listening = socket->createListener(portNumber, localHostName);
  }



  if (listening)
  {
    std::printf("Listening on %s:%d\n",localHostName,portNumber);
    if (true) {
      juce::MessageManagerLock mml (Thread::getCurrentThread());
      httpState->setText("Listening on port " + std::to_string(portNumber), juce::sendNotification);
    }
      
    int cutFreqCalc;

    while (!threadShouldExit())
    {
      std::unique_ptr<juce::StreamingSocket> connection(socket->waitForNextConnection());

      if (connection != nullptr) {
        char buffer[MAX_URL_SIZE] = {0};

        int bytesRead = connection->read(buffer, MAX_URL_SIZE - 1, false);
        if (bytesRead > 0) {
          buffer[bytesRead] = '\0';

          if (true) {
            juce::MessageManagerLock mml (Thread::getCurrentThread());
            httpState->setText("Recieving on port " + std::to_string(portNumber), juce::sendNotification);
          }

          // === EXTRACT BODY ===
          // crude way to extract JSON body (assumes POST)
          char* body = strstr(buffer, "\r\n\r\n");

          // Process the json into the wavetable we want
          cutFreqCalc = processJson(body, bias->load(), midpoint->load(), filterScalar->load());

          // Send minimal HTTP 200 OK back to client
          const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n\r\n";
          connection->write(response, static_cast<int>(strlen(response)));
        }
      connection->close();
      }
      // Apply to shared buffer (guarded by readyToSwap being false)

      cutFreq->store(cutFreqCalc);
    }

    socket->close();
  }
  else
  {
    juce::MessageManagerLock mml (Thread::getCurrentThread());
    httpState->setText("Failed to create listener!", juce::sendNotification);
  }
}

int SocketServer::processJson(char* body, int bias, int midpoint, float scalar) {
  int wavetableSize = 280;
  juce::AudioBuffer<float> wavetable = juce::AudioBuffer<float>();
  wavetable.setSize(1,wavetableSize);
  wavetable.clear();
  int outfreq = midpoint;
  int lowerTotal = 0;
  int upperTotal = 0;

  if (body != nullptr)
  {
    body += 4; // skip past the "\r\n\r\n"
    juce::String bodyStr(body);

    // parse JSON
    juce::var parsed = juce::JSON::parse(bodyStr);
    if (parsed.isObject())
    {
      juce::Array<juce::var> * harmonicsArray = parsed.getProperty ("harmonicSeries",juce::var()).getArray();
      for (auto& harmonic : *harmonicsArray) {
        float amp = harmonic.getProperty("Amp",juce::var());
        int freq = harmonic.getProperty("Freq",juce::var());
        float phase = harmonic.getProperty("Phase",juce::var());
        
        if (amp < 0) {
          amp = -amp;
        }

        if (freq > bias || -freq > bias) {
          
          upperTotal += amp;
        } else {
            lowerTotal += amp;
        }
      }

      std::string outstr = "";
      // Normalize data from -1 to 1
      float* data = wavetable.getWritePointer(0);
      float maxAmp = 0.0f;
      for (int i = 0; i < wavetable.getNumSamples(); ++i)
        maxAmp = std::max(maxAmp, std::abs(data[i]));
      if (maxAmp > 0.0f || true)
      {
        for (int i = 0; i < wavetable.getNumSamples(); ++i) {
          data[i] /= maxAmp;
          outstr.append(std::to_string(i) + "," + std::to_string(data[i]) + "\n");
        }
      }
    }


    outfreq =  (int)((((float)upperTotal / (float)lowerTotal) * (20000.0f - midpoint) * scalar ) + midpoint);
    printf("factor = %f\n", (((float)upperTotal / (float)lowerTotal) * (20000.0f - midpoint) * scalar ));
    printf("outfreq = %d, upperTotal = %d, lowerTotal = %d, midpoint = %d, scalar = %f\n", outfreq, upperTotal, lowerTotal, midpoint, scalar);
    if (outfreq <= 0) {
        outfreq = 20;
    }
  }


  return outfreq;
}
