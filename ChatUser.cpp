#include "ChatUser.h"

ChatUser::ChatUser(){
  this -> username = "username";
  this -> password = "password";
  this -> login = "login";
}

ChatUser::ChatUser(const ChatUser & chat_user){
 this->username = chat_user.username;
 this->password = chat_user.password;
 this->login = chat_user.login;
}

std::string ChatUser::getUsername(){
    return username;
}

std::string ChatUser::getPassword(){
  return password;
}

std::string ChatUser::getLogin(){
  return login;
}
