/*Добавьте в ваш проект чата логгирование сообщений от других участников чата и своих сообщений.

Для этого напишите класс Logger, через который будет проходить вся работа с файлом логов.
Сохраняйте сообщения в файл на диске, можете назвать его log.txt. При создании объекта
логгера должно производиться открытие файла, а при его разрушении — закрытие файла (концепт RAII).

Реализуйте в классе два метода:

        запись строки логов в файл;
        чтение одной строки из файла.

Конечно же, запись и чтение должны быть потокобезопасными — при одновременном вызове
нескольких потоков с записью в файл и чтением из файла, не должно происходить взаимоблокировок и гонок данных.

Не забывайте про принципы инкапсуляции: код, пользующийся вашими методами чтения и
записи, не должен знать про многопоточность.*/

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <shared_mutex>

class Logger{
public:
  Logger(); //здесь будет открываться файл
  ~Logger(); //здесь будет закрываться файл
  std::string ReadingFileString(); //чтение строки
  void WritingFileString(std::string write_stuff); //запись строки
private:
  std::fstream log_file;
  std::shared_mutex shar_mut;
};

Logger::Logger(){
  mute.lock();
  log_file.open("log.txt");
  mute.unlock();
}

Logger::~Logger(){
  mute.lock();
  log_file.close();
  mute.unlock();
}

std::string Logger::ReadingFileString(){
  std::string buffer_str;
  shar_mut.lock_shared();
  if (!log_file.is_open()){
    throw buffer_str;
  } else {
      std::getline(log_file, buffer_str);
    }
  shar_mut.unlock_shared();
  return buffer_str;
}

void Logger::WritingFileString(std::string write_stuff){
  shar_mut.lock();
  if (!log_file.is_open()){
    throw write_stuff;
  } else {
      log_file << write_stuff << std::endl;
  }
  shar_mut.unlock();
}

void MultiThread(){
  Logger reader;
  std::string write_stuff = "you are user";
  std::string write_stuff2 = "you are ellie";
  std::string write_stuff3 = "you are cat";
  std::string write_stuff4 = "you are doge";

  try{
    std::thread th1([&]() { reader.WritingFileString(write_stuff); });
    std::thread th2([&]() { reader.WritingFileString(write_stuff2); });
    std::thread th3([&]() { reader.WritingFileString(write_stuff3); });
    std::thread th4([&]() { reader.WritingFileString(write_stuff4); });
    std::thread th5([&]() { reader.WritingFileString("write some stuff"); });

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
  } catch(std::string exception) {
    std::cout << "Cannot open the file!" << std::endl;
  }

}

void ReadMultiThread(){
  Logger reader;

  try{
    std::thread th1([&]() { std::cout << reader.ReadingFileString() << std::endl; });
    std::thread th2([&]() { std::cout << reader.ReadingFileString() << std::endl; });
    std::thread th3([&]() { std::cout << reader.ReadingFileString() << std::endl; });
    std::thread th4([&]() { std::cout << reader.ReadingFileString() << std::endl; });
    std::thread th5([&]() { std::cout << reader.ReadingFileString() << std::endl; });

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
  } catch(std::string exception){
    std::cout << "Cannot open the file!" << std::endl;
  }

}

int main(){
  MultiThread();
  ReadMultiThread();
  return 0;
}
