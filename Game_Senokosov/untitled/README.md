# README - Progetto di Gioco
Questo progetto di gioco è stato sviluppato per creare un'esperienza di gioco coinvolgente con diverse funzionalità, tra cui classi di oggetti, giochi e livelli, giocatori, UI, nemici, commercianti e casse. Il progetto si basa su un design orientato agli oggetti e si integra con un motore di gioco esistente.
# Classi Implementate
## Classi di Oggetti
- Armor: rappresenta un'armatura equipaggiabile con attributi di difesa.
- Bow: rappresenta un arco equipaggiabile con attributi di attacco.
- Shield: rappresenta uno scudo equipaggiabile con attributi di difesa.
- Sword: rappresenta una spada equipaggiabile con attributi di attacco.
## Classi di Giochi e Livelli
- Game: gestisce la logica generale del gioco, inclusi eventi, transizioni tra i livelli e stato di gioco.
- Level: rappresenta un livello specifico del gioco, con caratteristiche, nemici e obiettivi unici.
## Classe Giocatore
- Player: rappresenta il personaggio controllato dal giocatore, con metodi per gestire movimento, attacco, inventario e altre interazioni.
## Classe UI
- UI: gestisce l'interfaccia utente del gioco, inclusi elementi grafici, menu, tasti di controllo e visualizzazione delle informazioni di gioco.
## Classi di Nemici
- Enemy: classe base per rappresentare un nemico, con attributi come salute, danno, livello e comportamento generale.
- Goblin: sottoclasse di Enemy, rappresenta un goblin con attributi specifici.
- Orc: sottoclasse di Enemy, rappresenta un orco con attributi specifici.
## Classe Commerciante
- Merchant: rappresenta un commerciante nel gioco, con attributi di posizione e metodi per gestire gli oggetti disponibili per la vendita.
## Classe Torace
- Chest: rappresenta una cassa nel gioco, con attributi di posizione e metodi per aprirla e ottenere un premio.
# Contributi
g++  Game.cpp Level.cpp main.cpp Merchant.cpp Chest.cpp Player.cpp Projectile.cpp UI.cpp Enemies/Enemy.cpp Enemies/Goblin.cpp Enemies/Orc.cpp Items/Armor.cpp Items/Bow.cpp Items/Item.cpp Items/Shield.cpp Items/Sword.cpp  -lncurses  -o  game