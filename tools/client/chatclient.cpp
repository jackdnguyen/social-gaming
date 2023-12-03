/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <unistd.h>

#include "ChatWindow.h"
#include "Client.h"


int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 8000\n";
    return 1;
  }

  networking::Client client{argv[1], argv[2]};

  auto onTextEntry = [&client] (std::string text) {
    client.send(text);
  };

  ChatWindow chatWindow(onTextEntry);
  while (!client.isDisconnected()) {
    try {
      client.update();
    } catch (std::exception& e) {
      chatWindow.displayText("Exception from Client update:", false);
      chatWindow.displayText(e.what(), false);
    }

    auto [response, isSystemMessage] = client.receive();
    if (!response.empty()) {
      chatWindow.displayText(response, isSystemMessage);
    }
    chatWindow.update();
  }

  return 0;
}

