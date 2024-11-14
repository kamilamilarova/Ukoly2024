#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

#define PI 3.14159265358979323846

class Lod {
private:
    int x = 0, y = 0;              // Pozice lodi
    int waypoint_x = 10, waypoint_y = 1;  // Výchozí pozice waypointu pro část 2
    int direction = 0;              // Směr lodi ve stupních: 0 - východ, 90 - jih, 180 - západ, 270 - sever

public:
    // Funkce pro rotaci waypointu
    void otoc_waypoint(int uhel, char smer) {
        double rad = uhel * (PI / 180.0);
        if (smer == 'R') rad = -rad;  // Negativní pro rotaci doprava
        
        int nova_waypoint_x = round(cos(rad) * waypoint_x - sin(rad) * waypoint_y);
        int nova_waypoint_y = round(sin(rad) * waypoint_x + cos(rad) * waypoint_y);
        
        waypoint_x = nova_waypoint_x;
        waypoint_y = nova_waypoint_y;
    }

    // Funkce pro zpracování navigačních instrukcí
    int naviguj(const string& cesta_soubor, bool druhe_reseni) {
        ifstream file(cesta_soubor);
        if (!file.is_open()) {
            cerr << "Nepodařilo se otevřít soubor!" << endl;
            return -1;
        }

        string instruction;
        while (getline(file, instruction)) {
            char akce = instruction[0];
            int hodnota = stoi(instruction.substr(1));

            if (druhe_reseni) {  // Druhá část zadání s waypointem
                switch (akce) {
                    case 'N': 
                        waypoint_y += hodnota; 
                        break;
                    case 'S': 
                        waypoint_y -= hodnota; 
                        break;
                    case 'E': 
                        waypoint_x += hodnota; 
                        break;
                    case 'W': 
                        waypoint_x -= hodnota; 
                        break;
                    case 'L': 
                        otoc_waypoint(hodnota, 'L'); 
                        break;
                    case 'R': 
                        otoc_waypoint(hodnota, 'R'); 
                        break;
                    case 'F':
                        x += waypoint_x * hodnota;
                        y += waypoint_y * hodnota;
                        break;
                }
            } else {  // První část zadání bez waypointu
                switch (akce) {
                    case 'N':
                         y += hodnota; 
                         break;
                    case 'S': 
                        y -= hodnota;
                        break;
                    case 'E':
                        x += hodnota; 
                        break;
                    case 'W': 
                        x -= hodnota; 
                        break;
                    case 'L': 
                        direction = (direction + 360 - hodnota) % 360; 
                        break;
                    case 'R': 
                        direction = (direction + hodnota) % 360; 
                        break;
                    case 'F':
                        switch (direction) {
                            case 0: 
                                x += hodnota; 
                                break;    // Východ
                            case 90: 
                                y -= hodnota; 
                                break;   // Jih
                            case 180: 
                                x -= hodnota; 
                                break;  // Západ
                            case 270: 
                                y += hodnota; 
                                break;  // Sever
                        }
                        break;
                }
            }
        }
        file.close();

        // Vrací Manhattan vzdálenost (součet absolutních hodnot souřadnic) od počátečního bodu
        return abs(x) + abs(y);
    }
};

#ifndef __TEST__
int main() {
    // První část: navigace bez waypointu
    Lod lod;
    cout << "Manhattan vzdálenost (část 1): " << lod.naviguj("vstup_1.txt", false) << endl;

    // Druhá část: navigace s waypointem
    Lod lod2;
    cout << "Manhattan vzdálenost (část 2): " << lod2.naviguj("vstup_1.txt", true) << endl;

    return 0;
}
#endif // __TEST__