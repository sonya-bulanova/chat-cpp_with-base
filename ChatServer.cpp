#include "system_connection.h"

int ChatServer(){
  #if defined __linux__
  socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_file_descriptor == -1){
    std::cout << "Socket creation failed!" << std::endl;
    return(1);
  }

  serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddress.sin_port = htons(PORT);
  serveraddress.sin_family = AF_INET;
  bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

  if(bind_status == -1){
    std::cout << "Socket binding failed!" << std::endl;
    return(1);
  }

  connection_status = listen(socket_file_descriptor, 15);
  if (connection_status == -1){
    std::cout << "Socket is unable to listen for new connections!" << std::endl;
    return(1);
  } else {
    std::cout << "Server is listening for new connection: " << std::endl;
  }

  length = sizeof(client);
  connection = accept(socket_file_descriptor,(struct sockaddr*)&client, &length);
  if (connection == -1){
    std::cout << "Server is unable to accept the data from client!" << std::endl;
    return(1);
  }
  return 0;
  #else
  //код для сокета в винде
  #endif
}
