
# MySQLConnect

`MySQLConnect` est une classe C++ qui facilite la connexion à une base de données MySQL et la réalisation d'opérations CRUD (Create, Read, Update, Delete).

# Installation

Assurez-vous d'avoir installé `MySQL Connector/C++` avant de compiler ce projet. Vous pouvez le télécharger depuis [le site officiel de MySQL](https://dev.mysql.com/downloads/connector/cpp/).

# Utilisation

## Construction de la classe

Vous pouvez créer une instance de `MySQLConnect` avec les paramètres par défaut ou en spécifiant l'hôte, le nom d'utilisateur et le mot de passe.

```cpp
#include "MySQLConnect.h"

// Connexion avec les paramètres par défaut
MySQLConnect dbConnect;

// Connexion avec des paramètres spécifiques
MySQLConnect dbConnect("tcp://127.0.0.1:3306", "your_username", "your_password");
```
# Méthodes
## _set_db
Définit la base de données à utiliser.

```cpp
bool _set_db(const std::string& db_name);
```
*Paramètre*:

 - `db_name` - Nom de la base de données à utiliser.
 
*Retourne*:
 - `true` si la base de données a été définie avec succès, sinon `false`.
## _create
Insère des données dans une table.

```cpp
bool _create(const std::string& table, std::initializer_list<std::string> columns_lst, std::initializer_list<std::string> values_lst);
```
*Paramètres*:

 - `table` - Nom de la table dans laquelle insérer les données.
 - `columns_lst` - Liste des colonnes dans lesquelles les valeurs seront
   insérées.
 - `values_lst` - Liste des valeurs à insérer dans les colonnes
   spécifiées.

  *Retourne*: 
 - `true` si l'insertion a réussi, sinon `false`.

*Exemple*:
```cpp
dbConnect._create("tasks", {"title", "description", "due_date", "is_completed", "category_id"}, {"Task 1", "Description of task 1", "2024-12-31", "1", "2"});
```
## _read
Lit des données d'une table.

```cpp
sql::ResultSet* _read(const std::string& table, std::initializer_list<std::string> columns_lst);
```
*Paramètres*:

 - `table` - Nom de la table à lire.
 - `columns_lst` - Liste des colonnes à lire. Utilisez "*" pour lire
   toutes les colonnes.

*Retourne*: 
 - Un pointeur vers `sql::ResultSet` contenant les résultats de la
   requête.

*Exemple*:

```cpp
sql::ResultSet* res = dbConnect._read("tasks", {"title", "description"});
```

## _update
Met à jour les données dans une table.

```cpp
bool _update(const std::string& table, std::initializer_list<std::string> columns_lst, std::initializer_list<std::string> values_lst, const std::string& condition);
```
*Paramètres*:

 - `table` - Nom de la table à mettre à jour.
 - `columns_lst` - Liste des colonnes à mettre à jour.
 - `values_lst` - Liste des nouvelles valeurs pour les colonnes
   spécifiées.
 - `condition` - Condition pour sélectionner les enregistrements à
   mettre à jour.

*Retourne*: 

 - `true` si la mise à jour a réussi, sinon `false`.

*Exemple*:
```cpp
dbConnect._update("tasks", {"title", "description"}, {"Updated Title", "Updated Description"}, "id = 1");
```
## _delete
Supprime des données d'une table.

```cpp
bool _delete(const std::string& table, const std::string& condition);
```
*Paramètres*:

 - `table` - Nom de la table d'où supprimer les données.
 - `condition` - Condition pour sélectionner les enregistrements à
   supprimer.

*Retourne*: 

 - `true` si la suppression a réussi, sinon `false`.

*Exemple*:
```cpp
dbConnect._delete("tasks", "id = 1");
```
## _execute
Exécute une requête SQL avec des paramètres.

```cpp
sql::ResultSet* _execute(const std::string& query, std::initializer_list<std::string> values_lst, query_type type);
```
**Paramètres**:

 - `query` - La requête SQL à exécuter.
 - `values_lst` - Liste des valeurs à insérer dans la requête.
 - `type` - Type de la requête `(DB_READ, DB_CREATE, DB_UPDATE, ou
   DB_DELETE)`.

*Retourne*: 
 - Un pointeur vers `cpp sql::ResultSet` si type est `DB_READ`, sinon
   `nullptr`.

*Exemple*:

```cpp
sql::ResultSet* res = dbConnect._execute("SELECT * FROM tasks WHERE title = ?", {"Task 1"}, DB_READ);
```
# Contributeur
Si vous souhaitez contribuer à ce projet, veuillez créer une branche, effectuer vos modifications et soumettre une demande de tirage (`pull request`).

Licence
Ce projet est sous licence GNU. Voir le fichier LICENSE pour plus de détails.
