// test_server.cpp
#include "IRCMessage.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <vector>

#define PORT 6667
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int set_non_blocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    return -1;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;

  // Création du socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    return 1;
  }

  // Pour pouvoir relancer rapidement le serveur après un Ctrl+C
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // Mode non-bloquant dès le début
  if (set_non_blocking(server_fd) == -1) {
    perror("fcntl server_fd");
    return 1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    return 1;
  }

  // Listen
  if (listen(server_fd, 10) < 0) {
    perror("listen");
    return 1;
  }

  std::cout << "Serveur de test en écoute sur le port " << PORT
            << " (non-bloquant avec select)" << std::endl;
  std::cout << "Connecte-toi avec : nc localhost 6667" << std::endl
            << std::endl;

  std::vector<int> client_sockets;
  fd_set readfds;
  char buffer[BUFFER_SIZE];

  while (true) {
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    int max_fd = server_fd;

    // Ajouter tous les clients au set
    for (int i = 0; i < client_sockets.size(); ++i) {
      FD_SET(client_sockets[i], &readfds);
      if (client_sockets[i] > max_fd)
        max_fd = client_sockets[i];
    }

    // Timeout de 3 secondes juste pour pouvoir Ctrl+C proprement
    struct timeval tv = {3, 0};
    int activity = select(max_fd + 1, &readfds, NULL, NULL, &tv);

    if (activity < 0 && errno != EINTR) {
      perror("select error");
      break;
    }

    // Nouvelle connexion ?
    if (FD_ISSET(server_fd, &readfds)) {
      int new_socket;
      int addrlen = sizeof(address);

      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
          perror("accept");
      } else {
        set_non_blocking(new_socket);
        client_sockets.push_back(new_socket);
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, ip_str, INET_ADDRSTRLEN);
        std::cout << "Nouvelle connexion ! Client fd=" << new_socket
                  << " IP=" << ip_str << ":" << ntohs(address.sin_port)
                  << std::endl;
      }
    }

    // Vérifier les données des clients existants
    for (size_t i = 0; i < client_sockets.size(); ++i) {
      int client_fd = client_sockets[i];

      if (FD_ISSET(client_fd, &readfds)) {
        int bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read <= 0) {
          // Déconnexion ou erreur
          if (bytes_read == 0) {
            std::cout << "Client fd=" << client_fd << " s'est déconnecté."
                      << std::endl;
          } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
            perror("recv");
          }
          close(client_fd);
          client_sockets.erase(client_sockets.begin() + i);
          --i;
        } else {
          buffer[bytes_read] = '\0';
          std::cout << "Reçu du client fd=" << client_fd << " (" << bytes_read
                    << " bytes) :\n>>> " << buffer << std::endl;

          IRCMessage::newMessage(buffer).print();

          // Echo de test (optionnel, tu peux commenter)
          send(client_fd, buffer, bytes_read, 0);
        }
      }
    }
  }

  close(server_fd);
  return 0;
}
