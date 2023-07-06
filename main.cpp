#include "ChatUser.h"
#include "ChatArray.h"
#include "system_connection.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#define SERVER

// Шаблонная функция для получения ввода от пользователя с обработкой исключений
template <typename T>
T getInput(const std::string& prompt) {
    std::cout << prompt;
    T input;
    std::getline(std::cin, input);
    while (std::cin.fail()) {
        std::cout << "Некорректный ввод. Пожалуйста, попробуйте еще раз." << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << prompt;
        std::cin >> input;
    }
    return input.c_str();
}

int main() {
    system_connection(); //подключаем определение системы...

    std::vector<ChatUser> user_database;
    std::string _name;
    std::string _login;
    std::string _password;

    std::ifstream UserFile("user.txt"); //наша "База данных" юзеров
    std::fstream MessageFile("message.txt"); //наша "База данных" сообщений

    system("chmod 600 user.txt"); //фиксируем права доступа на файлы...
    system("chmod 600 message.txt");

    if (!UserFile.is_open()){
      std::cout << "File was not open" << std::endl;
      return 1;
    }

    if (UserFile.peek() == EOF){ //если файл пустой, то создаем пустой чат
      std::cout << "Create default chat" << std::endl;
      ChatArray chat;
    } else { //если файл непустой, считываем данные и записываем их в массив
      while(UserFile >> _name >> _login >> _password){
        user_database.push_back(ChatUser(_name, _login, _password));
      }
      UserFile.close();

      //auto chat = new ChatArray();
      auto chat = new ChatArray(user_database);

    while (true) { // цикл для регистрации новых пользователей или входа в систему
        std::string choice = getInput<std::string>("Введите 'r', чтобы зарегистрировать нового пользователя, или 'l', чтобы войти в систему: ");
        try {
            if (choice == "r") { // если пользователь выбрал 'r', то регистрируем нового пользователя
                _name = getInput<std::string>("Введите новое имя пользователя: ");
                _login = getInput<std::string>("Введите логин: ");
                _password = getInput<std::string>("Введите новый пароль: ");
                chat->registerUser(_name, _password, _login);
            }
            else if (choice == "l") { // если пользователь выбрал 'l', то запрашиваем данные для входа в систему
                while (true) {
                    _name = getInput<std::string>("Введите имя пользователя: ");
                    _password = getInput<std::string>("Введите пароль: ");
                    if (chat->authenticateUser(_name, _password)) { // если данные для входа правильные, то выходим из цикла
                        break;
                    }
                    else {
                        std::cout << "Неверное имя пользователя или пароль. Попробуйте еще раз." << std::endl; // иначе сообщаем об ошибке
                    }
                }
                std::cout << "Вы вошли в систему как " << _name << "." << std::endl; // сообщаем о успешном входе

                while (true) { // основной цикл чата
                    std::string choice = getInput<std::string>("Введите 's', чтобы отправить сообщение, 'p', чтобы показать свои сообщения, или 'b', чтобы отправить сообщение всем: ");

                        try {
                        if (choice == "s") { // если пользователь выбрал 's', то запрашиваем получателя и сообщение
                            std::string recipient = getInput<std::string>("Введите имя получателя: ");
                            std::string message = getInput<std::string>("Введите ваше сообщение: ");
                            chat->sendMessage(_name, recipient, message); // отправляем сообщение
                            // начинаем процедуру отправки по сети, версия клиента
                            #if defined CLIENT
                            ClientChat();
                            if ((strncmp(message, "end", 3)) == 0) {
                              write(socket_file_descriptor, message, sizeof(message));
                              std::cout << "Client Exit." << std::endl;
                              break;
                            }
                            ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
                            // Если передали >= 0  байт, значит пересылка прошла успешно
                            if(bytes >= 0){
                              std::cout << "Data send to the server successfully..." << endl;
                            }
                            // Ждем ответа от сервера
                            read(socket_file_descriptor, message, sizeof(message));
                            std::cout << "Data received from server: " << message << std::endl;
                            // начинаем процедуру отправки по сети, версия сервера
                            #elif defined SERVER
                            ChatServer();
                            char *message_send = new char[message.length() + 1];
                            strcpy(message_send, message.c_str());

                            bzero(message_send, MESSAGE_LENGTH);
                            read(connection, message_send, sizeof(message_send));
                            if (strncmp("end", message_send, 3) == 0) {
                              std::cout << "Client Exited." << std::endl;
                              std::cout << "Server is Exiting..ю" << std::endl;
                              close(socket_file_descriptor);
                              return(1);
                            }
                            std::cout << "Data received from client: " <<  message << std::endl;
                            bzero(message_send, MESSAGE_LENGTH);
                            ssize_t bytes = write(connection, message_send, sizeof(message_send));
                            if(bytes >= 0)  {
                              std::cout << "Data successfully sent to the client.!" << std::endl;
                            }
                            #else
                            std::cout << "Offline messaging is avaliable" << std::endl;
                            #endif

                            std::cout << "Сообщение отправлено " << recipient << "." << std::endl;
                        }
                        else if (choice == "p") { // если пользователь выбрал 'p', то показываем все его сообщения
                            chat->printMessages(_name);
                        }
                        else if (choice == "b") { // если пользователь выбрал 'b', то запрашиваем сообщение и отправляем его всем пользователям
                            std::string message = getInput<std::string>("Введите сообщение для всех: ");
                            chat->broadcastMessage(_name, message);
                            std::cout << "Сообщение отправлено всем пользователям." << std::endl;
                        }
                    }
                    catch (const std::out_of_range& e) { // обрабатываем исключение при попытке доступа к сообщениям несуществующего пользователя
                        std::cerr << "Ошибка: " << e.what() << std::endl;
                    }
                }
                close(socket_file_descriptor);
                return 0;
            }
        }
        catch (const std::exception& e) { // обрабатываем любые другие исключения
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
}
    return 0;
}
