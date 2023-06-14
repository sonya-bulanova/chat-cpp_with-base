#include "ChatArray.h"
#include "ChatUser.h"

ChatArray::ChatArray(){
  std::cout << "Создан пустой чат." << std::endl;
}

ChatArray::ChatArray(std::vector<ChatUser> usrs){
  std::cout << "Загружен чат из базы данных." << std::endl;
  for (int i = 0; i < usrs.size(); i++){
    users.push_back(ChatUser(usrs[i].getUsername(), usrs[i].getLogin(), usrs[i].getPassword()));
  }
  loadMessageBase();
  //std::copy(usrs.begin(), usrs.end(), this -> users.begin());
}

ChatArray::ChatArray(ChatArray &ch){
  std::copy(this -> users.begin(), this -> users.end(), ch.users.begin());
}

ChatArray::~ChatArray(){
  if(users.size() != 0) {
    users.clear();
  }
  if(messages.size() != 0){
    messages.clear();
  }
}

void ChatArray::addUser(ChatUser user){
  users.push_back(user);
}

bool ChatArray::authenticateUser(std::string username, std::string password){
  for (int i = 0; i < users.size(); i++) {
      if (users[i].getUsername() == username && users[i].getPassword() == password) {
          return true;
      }
  }
  return false;
}

void ChatArray::registerUser(std::string username, std::string password, std::string login){
  ChatUser user(username, password, login);
  users.push_back(user);

  std::fstream UserFile("user.txt", std::ios::app);
  if( ! UserFile.is_open() ) {
    std::cout << "Could not open input file!" << std::endl;
  }
  UserFile << username << "\t" << login << "\t" << password << std::endl;
  UserFile.close();

  std::cout << "Пользователь " << username << " успешно зарегистрирован." << std::endl;
}

void ChatArray::sendMessage(std::string sender, std::string recipient, std::string message){
  messages[recipient].push_back(sender + ": " + message);

  std::fstream MessageFile("message.txt", std::ios::app);
  if( ! MessageFile.is_open() ) {
    std::cout << "Could not open message history file!" << std::endl;
  }
  MessageFile << sender << "\t" << recipient << "\t" << message << std::endl;
  MessageFile.close();

}

void ChatArray::printMessages(std::string recipient){
  std::cout << "Сообщения для " << recipient << ":" << std::endl;
  for (auto& message : messages[recipient]) {
      std::cout << message << std::endl;
  }
  std::cout << std::endl;
}

void ChatArray::broadcastMessage(std::string sender, std::string message){
  for (auto& user : users) {
      if (user.getUsername() != sender) {
          messages[user.getUsername()].push_back(sender + ": " + message);
      }
  }
  std::fstream MessageFile("message.txt", std::ios::app);
  if( ! MessageFile.is_open() ) {
    std::cout << "Could not open message history file!" << std::endl;
  }
  MessageFile << sender << "\t" << message << std::endl;
  MessageFile.close();
}

void ChatArray::loadMessageBase(){
  std::string buffer;
  std::string sender;
  std::string recipient;
  std::string message;

  std::fstream MessageFile("message.txt");
  if( ! MessageFile.is_open() ) {
    std::cout << "Could not open message history file!" << std::endl;
  }
  if (MessageFile.peek() == EOF){ //если файл пустой, то выводим сообщение о пустоте
    std::cout << "Message history is empty" << std::endl;
  } else {
    while(getline(MessageFile, buffer)){
      std::getline(MessageFile, sender, '\t');
      std::getline(MessageFile, recipient, '\t');
      std::getline(MessageFile, message);
      messages[recipient].push_back(sender + ": " + message);
    }
  }
}
