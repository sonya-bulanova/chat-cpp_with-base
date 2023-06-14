#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include "ChatUser.h"
#pragma once

class ChatArray {
private:
    std::vector<ChatUser> users;
    std::unordered_map<std::string, std::vector<std::string>> messages;
public:
    ChatArray(); //конструктор по умолчанию
    ChatArray(std::vector<ChatUser> user); //конструктор с параметрами
    ChatArray(ChatArray & ca); //конструктор копирования
    ~ChatArray();
    void addUser(ChatUser user);
    bool authenticateUser(std::string username, std::string password);
    void registerUser(std::string username, std::string password, std::string login);
    void sendMessage(std::string sender, std::string recipient, std::string message);
    void printMessages(std::string recipient);
    void broadcastMessage(std::string sender, std::string message);

    void loadMessageBase(); //функция, чтобы распарсить файл сообщений
};
