#include <string>
#pragma once

class ChatUser {
private:
    std::string username;
    std::string password;
    std::string login;

public:
    ChatUser();
    ChatUser(std::string username, std::string password, std::string login) : username(username), password(password), login(login) {}
    ChatUser(const ChatUser & chat_user); //конструктор копирования
    ~ChatUser(){};
    std::string getUsername();
    std::string getPassword();
    std::string getLogin();
};
