import sqlite3

conn = sqlite3.connect('database.db')
cursor = conn.cursor()

#Tabela de Temperatura
cursor.execute("""
    CREATE TABLE temperature (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        temperature TEXT NOT NULL,
        topic TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")

# Tabela de Umidade
cursor.execute("""
    CREATE TABLE humidity (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        humidity TEXT NOT NULL,
        topic TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")

#Tabela de Corrente RMS
cursor.execute("""
    CREATE TABLE currentrms (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        current TEXT NOT NULL,
        topic TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")

#Tabela de Comandos
cursor.execute("""
    CREATE TABLE comands (
        id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        comands TEXT NOT NULL,
        topic TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT current_timestamp
    );
""")

print('Tabelas criadas com sucesso')

conn.close()
