#include "system_connection.h"

int ClientChat(){

  #if defined __linux__
  socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_file_descriptor == -1){
    std::cout << "Creation of Socket failed!" << std::endl;
    exit(1);
  }

  // Установим адрес сервера
  serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
  // Зададим номер порта
  serveraddress.sin_port = htons(PORT);
  // Используем IPv4
  serveraddress.sin_family = AF_INET;
  // Установим соединение с сервером
  connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
  if(connection == -1){
    std::cout << "Connection with the server failed.!" << std::endl;
    exit(1);
  }
  return 0;

  #else
  //код для сокета в винде
  #endif
}
