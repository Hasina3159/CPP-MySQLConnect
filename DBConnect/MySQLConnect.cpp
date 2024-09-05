#include "MySQLConnect.h"

MySQLConnect::MySQLConnect() : driver(get_driver_instance())
{
    this->conn = std::unique_ptr<sql::Connection>(this->driver->connect("tcp://127.0.0.1:3306", "root", "root"));
};

MySQLConnect::MySQLConnect(const std::string& hostname, const std::string& username, const std::string& password) : driver(get_driver_instance())
{
    this->conn = std::unique_ptr<sql::Connection>(this->driver->connect(hostname, username, password));
};

MySQLConnect::~MySQLConnect()
{
    if (conn && conn->isValid())
    {
        conn->close();
    }
};

bool MySQLConnect::_set_db(const std::string& db_name)
{
    try
    {
        this->conn->setSchema(db_name);
        return (true);
    }
    catch (sql::SQLException)
    {
        return (false);
    }
};

bool MySQLConnect::_create(const std::string& table, std::initializer_list<std::string> columns_lst, std::initializer_list<std::string> values_lst)
{
    std::vector<std::string> columns(columns_lst);
    std::vector<std::string> values(values_lst);

    std::string query;
    query = "INSERT INTO " + table + " (";
    for (size_t i = 0; i < columns.size(); ++i)
    {
        query += columns[i];
        if (i != columns.size() - 1)
            query += ", ";
    }
    query += ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i)
    {
        query += "?";
        if (i != values.size() - 1)
            query += ", ";
    }
    query += ");";

    std::cout << "\nQUERY : " << query << std::endl;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(this->conn->prepareStatement(query));
        for (size_t i = 0; i < values.size(); ++i)
            pstmt->setString(i + 1, values[i]);

        pstmt->execute();
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error creating task: " << e.what() << std::endl;
        return (false);
    }

    return (true);
}

sql::ResultSet* MySQLConnect::_read(const std::string& table, std::initializer_list<std::string> columns_lst)
{
    std::vector<std::string> columns(columns_lst);
    std::string query;

    if (columns.size() == 1 && columns[0] == "*")
    {
        query = "SELECT * FROM " + table + ";";
    }
    else
    {
        query = "SELECT ";
        for (size_t i = 0; i < columns.size(); ++i)
        {
            query += columns[i];
            if (i != columns.size() - 1)
                query += ", ";
        }
        query += " FROM " + table + ";";
    }

    std::cout << "\nQUERY : " << query << std::endl;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(this->conn->prepareStatement(query));
        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next())
        {
            if (columns.size() == 1 && columns[0] == "*")
            {
                sql::ResultSetMetaData* metaData = res->getMetaData();
                int columnCount = metaData->getColumnCount();
                for (int i = 1; i <= columnCount; ++i)
                {
                    std::cout << metaData->getColumnName(i) << ": " << res->getString(i) << " ";
                }
            }
            else
            {
                for (const auto& col : columns)
                {
                    std::cout << col << ": " << res->getString(col) << " ";
                }
            }
            std::cout << std::endl;
        }

        return (res);
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Error reading from table: " << e.what() << std::endl;
        return (nullptr);
    }
}

bool MySQLConnect::_update(const std::string& table, std::initializer_list<std::string> columns_lst, std::initializer_list<std::string> values_lst, const std::string& condition)
{
    if (columns_lst.size() != values_lst.size())
    {
        throw std::invalid_argument("Le nombre de colonnes doit correspondre au nombre de valeurs.");
    }

    std::vector<std::string> columns(columns_lst);
    std::vector<std::string> values(values_lst);
    std::string query = "UPDATE " + table + " SET ";

    for (size_t i = 0; i < columns.size(); ++i)
    {
        query += columns[i] + " = ?";
        if (i != columns.size() - 1)
        {
            query += ", ";
        }
    }

    query += " WHERE " + condition + ";";

    std::cout << "\nQUERY : " << query << std::endl;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(this->conn->prepareStatement(query));

        for (size_t i = 0; i < values.size(); ++i)
        {
            pstmt->setString(i + 1, values[i]);
        }

        pstmt->executeUpdate();
        std::cout << "Mise a jour reussie." << std::endl;

        return (true);
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Erreur lors de la mise a jour: " << e.what() << std::endl;
        return (false);
    }
}
bool MySQLConnect::_delete(const std::string& table, const std::string& condition)
{
    std::string query = "DELETE FROM " + table + " WHERE " + condition + ";";

    std::cout << "\nQUERY : " << query << std::endl;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt(this->conn->prepareStatement(query));
        int affectedRows = pstmt->executeUpdate();

        if (affectedRows > 0)
        {
            std::cout << "Suppression reussie." << std::endl;
            return (true);
        }
        else
        {
            std::cout << "Aucun enregistrement trouve correspondant a la condition donnee." << std::endl;
            return (false);
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "Erreur lors de la suppression: " << e.what() << std::endl;
        return (false);
    }
}

sql::ResultSet* MySQLConnect::_execute(const std::string& query, std::initializer_list<std::string> values_lst, query_type type)
{
    std::vector<std::string> values(values_lst);
    std::unique_ptr<sql::PreparedStatement> pstmt(this->conn->prepareStatement(query));

    for (int index = 0; index < values.size(); index++)
    {
        pstmt->setString(index + 1, values[index]);
    }

    if (type == DB_READ)
    {
        return (pstmt->executeQuery());
    }
    else
    {
        pstmt->executeUpdate();
        return (nullptr);
    }
}
