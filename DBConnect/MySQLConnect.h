#pragma once

#include <string>
#include "mysql_connection.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

enum query_type
{
	DB_CREATE,
	DB_READ,
	DB_UPDATE,
	DB_DELETE
};

class MySQLConnect
{
private:
	sql::Driver* driver;
	std::unique_ptr<sql::Connection> conn;

public:
	MySQLConnect();
	MySQLConnect(const std::string& hostname, const std::string& username, const std::string& password);
	~MySQLConnect();
	bool _set_db(const std::string& db_name);
	bool _create(const std::string& table, std::initializer_list<std::string> columns,
		std::initializer_list<std::string> values);
	sql::ResultSet* _read(const std::string& table, std::initializer_list<std::string> columns_lst);
	bool _update(const std::string& table, std::initializer_list<std::string> columns_lst, std::initializer_list<std::string> values_lst, const std::string& condition);
	bool _delete(const std::string& table, const std::string& condition);
	sql::ResultSet* _execute(const std::string& query, std::initializer_list<std::string> columns_lst, query_type type);
};
