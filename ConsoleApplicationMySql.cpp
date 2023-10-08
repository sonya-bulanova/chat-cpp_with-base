/*Клиенту при своём старте необходимо связаться с сервером по IP-адресу сервера и порту, 
зарегистрироваться на нём, передав ему свой логин и пароль.

Если регистрация прошла успешно, то сервер в ответ высылает список пользователей 
и историю сообщений. Основным достоинством этой архитектуры является то, что все 
данные хранятся в одном месте, и так проще и надёжнее можно организовать целостность 
базы данных и синхронизацию данных между различными клиентами.*/

#include <iostream>
#include <mysql.h>
#include <vector>

using namespace std;

//необходимо создать две связанные БД. В первой хранятся пользователи, во второй -- их сообщения.

//Функция создания БД, имитируем чат

class DataBaseSQL {
public:
	DataBaseSQL(const char* ip_addr, const char* db_user, const char* basename, const char* password, unsigned int port); //создаем подключение
	~DataBaseSQL(); //отключаемся
	void CreateMessageBase(); //создаем БД, если она не создана
	void AddElement(std::string user, std::string address, std::string message); //добавляем строку (сообщение + юзер)
	void ShowAllContent(); //показываем всё содержимое
private:
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
};

DataBaseSQL::DataBaseSQL(const char* ip_addr, const char* db_user, const char* basename, const char* password, unsigned int port) {
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, ip_addr, db_user, password, basename, port, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	mysql_set_character_set(&mysql, "utf8");
}

DataBaseSQL::~DataBaseSQL() {
	mysql_close(&mysql);
}

void DataBaseSQL::CreateMessageBase() { //id, user, address, message. connected with table_of_users database
	mysql_query(&mysql, "CREATE TABLE table_user_messages(id INT AUTO_INCREMENT PRIMARY KEY, user_name VARCHAR(255) NOT NULL REFERENCES table_of_users(login), address VARCHAR(255), message VARCHAR(255))");
}

void DataBaseSQL::AddElement(std::string user, std::string address, std::string message) {
	std::string base_str("INSERT INTO table_user_messages(id, user, address, message) values(default, ' " + user + " ' " + "," + " ' " + address + " ' " + "," + " ' " + message + " ' " + ")");
	mysql_query(&mysql, base_str.c_str());
}

void DataBaseSQL::ShowAllContent() {
	mysql_query(&mysql, "SELECT * FROM table_user_messages");
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (int i = 0; i < mysql_num_fields(res); i++) {
				cout << row[i] << "  ";
			}
			cout << endl;
		}
	}
	else {
		std::cout << "MySql error number: " << mysql_error(&mysql);
	}
}

class DataBaseUsers {
public:
	DataBaseUsers(); //create DB of users
	~DataBaseUsers(); 
	void RegisterUser(std::string login, std::string username, std::string password); //register a user
	void ShowUsers(); //show all usernames
	void CreateUsersBase();
	void EnterInChat(std::string login, std::string password); //check if login and password correct
private:
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
};

DataBaseUsers::DataBaseUsers() {
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "alice2799", "testdb", 3306, NULL, 0)) { //enter to server
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	mysql_set_character_set(&mysql, "utf8");
}

DataBaseUsers::~DataBaseUsers() {
	mysql_close(&mysql);
}

void DataBaseUsers::CreateUsersBase() { //we need to create the DB only once, when we start a chat
	mysql_query(&mysql, "CREATE TABLE table_of_users(id INT AUTO_INCREMENT PRIMARY KEY, login VARCHAR(255), username VARCHAR(255), password VARCHAR(255), UNIQUE(login))");
}

void DataBaseUsers::RegisterUser(std::string login, std::string username, std::string password) { //call everytime you want to register a user
	std::string base_str("INSERT INTO table_of_users(id, login, username, password) values(default, ' " + login + " ' " + "," + " ' " + username + " ' " + "," + " ' " + password + " ' " + ")");
	mysql_query(&mysql, base_str.c_str());
}

void DataBaseUsers::ShowUsers() {
	mysql_query(&mysql, "SELECT * FROM table_of_users");
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			for (int i = 0; i < mysql_num_fields(res); i++) {
				std::cout << row[i] << "  ";
			}
			std::cout << endl;
		}
	}
	else {
		std::cout << "MySql error number: " << mysql_error(&mysql);
	}
}

void DataBaseUsers::EnterInChat(std::string check_login, std::string check_password) {
	mysql_query(&mysql, "SELECT * FROM table_of_users");
	std::string login, password;
	std::vector<std::string> login_users;
	std::vector<std::string> password_users;
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res))
		{
			for (auto i = 1; i < mysql_num_fields(res); ++i)
			{
				login = row[i];
				password = row[i + 2];
				login_users.push_back(login);
				password_users.push_back(password);
				break;
			}
		}
	}

	for (int i = 0; i < login_users.size(); i++) {
		if ((check_login == login_users[i]) && (check_password == password_users[i])) {
			std::cout << "Welcome to chat! Here is Message story: " << std::endl;
			DataBaseSQL * DBMes = new DataBaseSQL("127.0.0.1", "root", "testdb", "alice2799", 3306);
			DBMes->ShowAllContent();
		}
	}
}



int main() {
	DataBaseUsers* usrs = new DataBaseUsers();
	/*usrs->CreateUsersBase();

	usrs->RegisterUser("AenEllie", "Alice de Arago", "ellie2799");
	usrs->RegisterUser("AenEllie", "Alice de Arago", "ellie2799");
	usrs->RegisterUser("Fraeder", "Vladislav Suslov", "ghdkekmm303f");
	usrs->RegisterUser("Jay", "Josephine Franklin", "666voodoo666");
	usrs->RegisterUser("Evg", "Eugene de Arago", "mysisheck");
	usrs->RegisterUser("JB", "Jean Baptiste la Fabbro", "guitarhero");*/

	//usrs->ShowUsers();
	usrs->EnterInChat("JB", "guitarhero");

	/*const char* ip_address = "127.0.0.1";
	const char* db_user = "root";
	const char* basename = "testdb";
	const char* password = "alice2799";
	unsigned int port = 3306;

	DataBaseSQL * db = new DataBaseSQL(ip_address, db_user, basename, password, port);
	db->CreateMessageBase();

	db->AddElement("Alice", "Eugene", "Hello, heck!");
	db->AddElement("Eugene", "Alice", "Hi, sis!");
	db->AddElement("Josephine", "Alice", "Lets go to Warcraft");
	db->AddElement("Jean_Baptiste", "Josephine", "Better Minecraft");

	db->ShowAllContent();
	delete db;*/
 
	return 0;
}