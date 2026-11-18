# 🎓 Smart Student RFID Card System (Wokwi Simulation)

## 📌 Description du Projet
Ce projet simule un système intelligent de contrôle d'accès et de suivi de présence pour les examens universitaires en utilisant la technologie RFID (Arduino et module MFRC522) sur la plateforme **Wokwi**.

Le système gère des règles spécifiques par badge :
* **1er Scan (Étudiant) :** Détection et enregistrement de l'entrée à l'examen.
* **2ème Scan (Étudiant) :** Enregistrement de la sortie de l'examen.
* **3ème Scan (Étudiant) :** Blocage automatique de la carte (tentative non autorisée ou fin de droit).
* **Carte Enseignant :** Badge spécifique permettant de valider et d'enregistrer la présence de l'instructeur.

---

## 🎥 Démonstration Vidéo
Voici une démonstration du fonctionnement du système en simulation :
> https://github.com/user-attachments/assets/20632648-b2bb-4cb3-b7db-c1c99ecaad9d



---

## 🛠️ Composants Matériels (Simulation Wokwi)
* **Microcontrôleur :** Arduino Uno
* **Lecteur RFID :** RC522 (Interface SPI)
* **Interface & Alertes :** Écran LCD I2C (16x2), LEDs de statut (Vert/Rouge), Buzzer.

---

## 📂 Structure du Dépôt
```text
├── sketch.ino          # Code source gérant la logique des scans et états des cartes
├── diagram.json        # Schéma de câblage et connexions sur Wokwi
├── libraries.txt       # Liste des bibliothèques requises (MFRC522, LiquidCrystal_I2C)
└── README.md           # Documentation du projet
