#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"

void Menu();

/*
    SDP Training Minigame #3
  
    There are two circles – one on the left, one on the right.​
    The two circles go from one side to the opposite side.​
    Make it so that when the two circles collide, they disappear.​
*/
void Menu() {
    
    int x, y;
    
    while (1) { //always run
        //menu
        FEHImage image("menu.png");
        image.Draw(0, 0);
        LCD.SetFontColor(SCARLET);
        LCD.WriteAt("Play", 27, 70);
        LCD.SetFontColor(GREEN);
        LCD.WriteAt("How", 30, 140);
        LCD.WriteAt("2", 30, 160);
        LCD.WriteAt("Play", 30, 180);
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Stats", 97, 70);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Creds", 97, 160);
        LCD.SetFontColor(YELLOW);
        LCD.WriteAt("NutriBros", 203, 120);

        //wait for touch
        while (!LCD.Touch(&x, &y)) {};

        //which button clicked
        if (x > 14 && x < 88 && y > 25 && y < 118) { //play
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Play game here", 0, 0);
            LCD.SetFontColor(RED);
            LCD.WriteAt("Back", 0, 220);

            //if back clicked
            while (1) {
                if (LCD.Touch(&x, &y)) {
                    if (x > 0 && x < 50 && y > 220 && y < 240) {
                        LCD.Clear();
                        Sleep(0.1);
                        break; //back to menu
                    }
                }
            }
        } else if (x > 14 && x < 88 && y > 125 && y < 217) { //how 2 play
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("1. Move around by clicking!");
            LCD.WriteLine("2. Dodge the unhealthy foods!");
            LCD.WriteLine("3. Eat (touch) the healthy foods!");
            LCD.SetFontColor(RED);
            LCD.WriteAt("Back", 0, 220);

            //if back clicked
            while (1) {
                if (LCD.Touch(&x, &y)) {
                    if (x > 0 && x < 50 && y > 220 && y < 240) {
                        LCD.Clear();
                        Sleep(0.1);
                        break; //back to menu
                    }
                }
            }
        } else if (x > 93 && x < 166 && y > 25 && y < 118) { //statistics
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("You reached level 1");
            LCD.WriteLine("You ate 3 healthy foods");
            LCD.WriteLine("You dodged 15 unhealthy foods");
            LCD.SetFontColor(RED);
            LCD.WriteAt("Back", 0, 220);

            //if back clicked
            while (1) {
                if (LCD.Touch(&x, &y)) {
                    if (x > 0 && x < 50 && y > 220 && y < 240) {
                        LCD.Clear();
                        Sleep(0.1);
                        break; //back to menu
                    }
                }
            }
        } else if (x > 93 && x < 166 && y > 125 && y < 217) { //credits
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Developers:");
            LCD.SetFontColor(CYAN);
            LCD.WriteLine("Daniel Chae");
            LCD.SetFontColor(GREEN);
            LCD.WriteLine("Sky Sie");
            LCD.SetFontColor(RED);
            LCD.WriteAt("Back", 0, 220);

            //if back clicked
            while (1) {
                if (LCD.Touch(&x, &y)) {
                    if (x > 0 && x < 50 && y > 220 && y < 240) {
                        LCD.Clear();
                        Sleep(0.1);
                        break; //back to menu
                    }
                }
            }
        }
    } 
}

int main() {
    //Clear background

    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    Menu();
    
    while(1) {
        LCD.Update(); //Never quit 
    }
    return 0;
}