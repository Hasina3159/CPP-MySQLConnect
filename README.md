# MySQLConnect

`MySQLConnect` est une classe C++ qui facilite la connexion à une base de données MySQL et la réalisation d'opérations CRUD (Create, Read, Update, Delete).

## Installation

Assurez-vous d'avoir installé MySQL Connector/C++ avant de compiler ce projet. Vous pouvez le télécharger depuis [le site officiel de MySQL](https://dev.mysql.com/downloads/connector/cpp/).

## Utilisation

### Construction de la classe

Vous pouvez créer une instance de `MySQLConnect` avec les paramètres par défaut ou en spécifiant l'hôte, le nom d'utilisateur et le mot de passe.

```cpp
#include "MySQLConnect.h"

// Connexion avec les paramètres par défaut
MySQLConnect dbConnect;

// Connexion avec des paramètres spécifiques
MySQLConnect dbConnect("tcp://127.0.0.1:3306", "your_username", "your_password");
