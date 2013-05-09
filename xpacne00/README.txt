Jiří Navrátil (xnavra360
Jan Pacner (xpacne00)

Makefile vyuziva prikaz "qmake", a je tedy nutne nastavit spravne $PATH pred spustenim "make"

CLI
  spustit pomoci
    make run-cli
  dale viz. napoveda prikazem h

GUI
  spustit pomoci
    make run
  dale vse zrejme z GUI

problemy:
  ulozeni, ukonceni a znovunacteni sitove hry zpusobi, ze se ulozi port na vzdalenem serveru - tedy po restartu serveru nemusi jit nacist a spustit sitovou hru spustit, protoze server mezitim nekdo mohl restartovat a tento pobezi zrejme na jinem portu (resenim je rucni editace XML nebo prenastaveni portu v uzivatelskem rozhrani)
  bezpecnost sitove komunikace neresime

vim: set wrap:
