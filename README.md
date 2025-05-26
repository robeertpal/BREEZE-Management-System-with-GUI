BREEZE MANAGEMENT SYSTEM (BMS) realizat de Pal Robert-Attila

BREEZE Management System este o aplicație CLI pentru administrarea participanților, personalului intern și partenerilor unui festival de muzică.

1. INSTALARE & EXECUTARE

   Dependențe:
        - Compiler: g++ (versiune 7.0 sau mai nouă)
        - Bibliotecă JSON: nlohmann/json (header-only)
        - Qt 6.9.0

   Instalare nlohmann/json (macOS):
        brew install nlohmann-json

   Instalare Qt 6.9.0 (macOS):
        licență qt.io

   Compilare GUI:
         /Users/robert/Qt/6.9.0/macos/bin/qmake breeze.pro
         make clean
         make

   Rulare:
         ./breeze.app/Contents/MacOS/breeze

2. FUNCȚIONALITATE DETALIATĂ
   Aplicația oferă două moduri de lucru:

    Agent BREEZE

       TicketAgent:
            Afișează toți participanții
            Afișează detalii despre un participant (prenume, nume, email, telefon, dată naștere, naționalitate)
            Afișează detalii despre bilet (tip și cod)
            Adaugă participant nou
                - Introduce date personale
                - Se aplică validări pentru email, telefon și vârstă (>=18 ani)
                - Se generează cod “BREEZE-<TT>-<NumărUnic>” (unde "TT" reprezintă tipul de bilet iar "NumărUnic" este un cod generat pe baza numelui, prenumelui și data nașterii)
                - Se salvează în fișierul guests.json
            Realizează check-in pentru participant și actualizează fișierul guests.json

       SecurityAgent:
            Inspectează lista participanților
            Afișează detalii privind un participant
            Afișează detalii privind biletul
            Restricționează accesul (șterge participant din guests.json)

       Top-Up Agent:
            Încarcă sold pe brățară
            Afișează soldul brățării

       Admin BREEZE (staff ID-ul este 0):
          - Are acces la toate operațiunile de mai sus
          - Management Staff: adaugă și șterge agenți, afișează staff-ul pe departamente, modifică detaliile unui agent (prenume, nume, dată naștere, telefon, naționalitate, rol)
          - Gestionare credentiale: modifică propriul username și parola dar și credentialele altor agenți

    Partener
       După autentificare, rolurile posibile sunt Admin Partener (staff ID-ul este 0) și Worker Partener.

        Worker Partener:
            Înregistrează vânzare:
             - Introduce codul biletului clientului
             - Debitează suma de pe brățara clientului (arată eroare la fonduri insuficiente)
             - Creditează contul partenerului și actualizează partners.json
            Consultă soldul partenerului
            Modifică propriile credentiale (username, parolă)

        Admin Partener:
            Are toate funcționalitățile Worker Partener
            Creează parteneri noi (nume, tip, sold inițial)
            Creează angajați (workers) și le definește credentialele
            Modifică credentialele proprii și ale workers

3. PRINCIPII OOP
    Moștenire și polimorfism: clase abstracte Ticket și Staff cu metode virtuale pure
    Constructor virtual (clone) în Email și PhoneNumber pentru copiere polimorfă
    Smart pointers (std::shared_ptr) și dynamic_cast pentru downcast sigur
    Copy-and-swap pentru operator=
    Excepții proprii: InvalidEmail, InvalidPhoneNumber, InvalidDate, AgeRestriction, InvalidTicket, WeakPassword, InsufficientFunds, UsernameAlreadyExists
    Validatori dedicați pentru email, telefon, dată, cod brățară și vârstă

4. EXEMPLU DE FLUX
   TicketAgent adaugă participant:
        - Completează datele ➔ validări automate
        - Se generează cod BREEZE-GA-123456789
   TopUpAgent încarcă 100 lei pe brățară ➔ sold brățară = 100 lei
   Admin Partener înregistrează vânzare:
        - Cod bilet: BREEZE-GA-123456789
        - Sumă: 30 lei ➔ brățară client = 70 lei, cont partener = +30 lei

5. SURSE 
     https://www.geeksforgeeks.org/dynamic-_cast-in-cpp/
     https://learn.microsoft.com/en-us/cpp/cpp/dynamic-cast-operator?view=msvc-170
     https://github.com/nlohmann/json?tab=readme-ov-file#json-pointer-and-json-patch
     https://en.cppreference.com/w/cpp/container/vector
     https://cplusplus.com/reference/algorithm/
     https://github.com/mcmarius/poo/blob/master/README.md

6. NOTĂ FINALĂ
   Toate datele persistă în fișiere JSON între rulări. Proiectul este modular, ușor extensibil cu noi roluri, validări sau integrare API.


