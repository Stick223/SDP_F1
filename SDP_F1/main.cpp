#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "FEHRandom.h"

#define SCREENW 320
#define SCREENH 240
#define CHARW 20
#define CHARH 40
#define SPRITEW 10
#define SPRITEH 10

int healthyFoodsEaten = 0;
int unhealthyFoodsEaten = 0;
int levelReached = 1;
// array of food images
FEHImage foods[8] = {FEHImage("apple.png"), FEHImage("avocado.png"), FEHImage("carrot.png"), FEHImage("chicken.png"),
                     FEHImage("hamburger.png"), FEHImage("soda.png"), FEHImage("candy.png"), FEHImage("cookie.png")};
void Level1();
void Level2();
void Level3();
// void Level4(); FOR LATER
// void Level5(); FOR LATER
void Menu();

void ShowStats()
{
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Game Statistics", 80, 20);
    LCD.WriteAt("Level Reached:", 10, 60);
    LCD.WriteAt(levelReached, 190, 60);
    LCD.WriteAt("Healthy Foods Eaten:", 10, 100);
    LCD.WriteAt(healthyFoodsEaten, 260, 100);
    LCD.WriteAt("Unhealthy Foods Eaten:", 10, 140);
    LCD.WriteAt(unhealthyFoodsEaten, 280, 140);
    LCD.SetFontColor(RED);
    LCD.WriteAt("Back", 0, 220);

    // back button
    int x, y;
    while (1)
    {
        if (LCD.Touch(&x, &y))
        {
            if (x > 0 && x < 50 && y > 220 && y < 240)
            {
                LCD.Clear();
                Sleep(0.1);
                break;
            }
        }
    }
}

void Level1()
{
    int x, y, x0 = SCREENW / 2, y0 = SCREENH / 2; // player position
    float total_time = TimeNow();                 // track time for object update
    float current_time = 0, elapsed_time = 0.0;

    int object_x = 0, object_y = -SPRITEH; // initialize object start position
    float object_move_time = 0.0;          // timing
    bool object_visible = false;           // object currently on screen?
    int current_food_index = 0;
    int score = 50; // START SCORE = 50

    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();

    FEHImage character_image("character2.png");

    character_image.Draw(x0, y0); // draw initial character

    // draw initial score
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Score:", SCREENW - 120, 5);
    LCD.WriteAt(score, SCREENW - 40, 5);

    while (1)
    {
        current_time = TimeNow();
        elapsed_time = current_time - total_time;
        total_time = current_time; // time keeping for object animations later

        if (score >= 75) // character weight progression
        {
            character_image = FEHImage("character1.png");
        }
        else if (score >= 50)
        {
            character_image = FEHImage("character2.png");
        }
        else if (score >= 40)
        {
            character_image = FEHImage("character3.png");
        }
        else if (score >= 25)
        {
            character_image = FEHImage("character4.png");
        }
        else if (score >= 10)
        {
            character_image = FEHImage("character5.png");
        }
        else
        {
            character_image = FEHImage("character6.png");
        }

        character_image.Draw(x0, y0);

        if (!object_visible)
        {
            object_x = Random.RandInt() % (SCREENW - 60); // randomize starting position at top
            object_y = -SPRITEH;
            current_food_index = Random.RandInt() % 8; // randomize food
            object_visible = true;                     // now object is visible. LIMIT OF 1 OBJECT FOR FUNCTIONALITY
        }

        // speed of 5 pixels per 0.03 seconds
        if (object_visible)
        {
            object_move_time += elapsed_time;
            if (object_move_time >= 0.03)
            {
                LCD.SetFontColor(WHITE); // erase previous object for animation illusion
                LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);

                object_y += 5;          // gravity effect
                object_move_time = 0.0; // reset move timer

                // new position unless off screen, then mark as not visible
                if (object_y <= SCREENH)
                {
                    foods[current_food_index].Draw(object_x, object_y);
                }
                else
                {
                    object_visible = false;
                }

                // redraw score because it keeps getting erased by objects
                LCD.SetFontColor(WHITE);
                LCD.FillRectangle(SCREENW - 120, 5, 120, 20);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Score:", SCREENW - 120, 5);
                LCD.WriteAt(score, SCREENW - 40, 5);
            }
        }

        // collision check
        if (object_visible && abs((object_x + SPRITEW / 2) - (x0 + CHARW / 2)) < CHARW / 2 && abs((object_y + SPRITEH / 2) - (y0 + CHARH / 2)) < CHARH / 2)
        {
            LCD.SetFontColor(WHITE); // erase food on collision
            LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);
            object_visible = false; // mark food as not visible

            // update score
            if (current_food_index < 4) // HEALTHY FOODS
            {
                score += 5;
                healthyFoodsEaten++;
            }
            else // UNHEALTHY FOODS
            {
                score -= 5;
                unhealthyFoodsEaten++;
            }

            // real score update
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(SCREENW - 40, 5, 40, 20);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Score:", SCREENW - 120, 5);
            LCD.WriteAt(score, SCREENW - 40, 5);
        }

        // next level indicator
        if (score >= 100)
        {
            levelReached++;
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Unhealthy objects now give -10 points!"); // next level instructions
            Sleep(3.0);
            LCD.SetBackgroundColor(LIGHTYELLOW);
            LCD.Clear();
            Level2(); // go to level 2
            break;
        }
        // game over indicator
        if (score <= 0)
        {
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            character_image = FEHImage("Gravestone.png");
            character_image.Draw(x0, y0);
            Sleep(1.5);
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.WriteLine("Game Over :(");
            Sleep(3.0);
            ShowStats();
            break;
        }
        // player movement (CLICK AND DRAG)
        if (LCD.Touch(&x, &y))
        {
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            x0 = x - CHARW / 2;
            y0 = y - CHARH / 2;

            // just made it to keep within bounds. Issues with frame rate and mouse speed handling.
            if (x0 < 0)
                x0 = 0;
            if (x0 + CHARW > SCREENW)
                x0 = SCREENW - CHARW;
            if (y0 < 0)
                y0 = 0;
            if (y0 + CHARH > SCREENH)
                y0 = SCREENH - CHARH;

            character_image.Draw(x0, y0);
        }

        Sleep(0.01); // buffer so the simulator doesn't blow up
    }
}

void Level2()
{
    /*
                            ***********
    refer to Level1(); for most of the documentation. The game functions are mostly the same
                            ***********
    */
    int x, y, x0 = SCREENW / 2, y0 = SCREENH / 2;
    float total_time = TimeNow();
    float current_time = 0, elapsed_time = 0.0;

    int object_x = 0, object_y = -SPRITEH;
    float object_move_time = 0.0;
    bool object_visible = false;
    int current_food_index = 0;
    int score = 50;

    LCD.SetBackgroundColor(LIGHTYELLOW);
    LCD.Clear();

    FEHImage character_image("character2.png");

    character_image.Draw(x0, y0);

    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Score:", SCREENW - 120, 5);
    LCD.WriteAt(score, SCREENW - 40, 5);

    while (1)
    {
        current_time = TimeNow();
        elapsed_time = current_time - total_time;
        total_time = current_time;

        if (score >= 75)
        {
            character_image = FEHImage("character1.png");
        }
        else if (score >= 50)
        {
            character_image = FEHImage("character2.png");
        }
        else if (score >= 40)
        {
            character_image = FEHImage("character3.png");
        }
        else if (score >= 25)
        {
            character_image = FEHImage("character4.png");
        }
        else if (score >= 10)
        {
            character_image = FEHImage("character5.png");
        }
        else
        {
            character_image = FEHImage("character6.png");
        }

        character_image.Draw(x0, y0);

        if (!object_visible)
        {
            object_x = Random.RandInt() % (SCREENW - 60);
            object_y = -SPRITEH;
            current_food_index = Random.RandInt() % 8;
            object_visible = true;
        }

        if (object_visible)
        {
            object_move_time += elapsed_time;
            if (object_move_time >= 0.03)
            {
                LCD.SetFontColor(LIGHTYELLOW);
                LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);

                object_y += 5;
                object_move_time = 0.0;

                if (object_y <= SCREENH)
                {
                    foods[current_food_index].Draw(object_x, object_y);
                }
                else
                {
                    object_visible = false;
                }

                LCD.SetFontColor(LIGHTYELLOW);
                LCD.FillRectangle(SCREENW - 120, 5, 120, 20);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Score:", SCREENW - 120, 5);
                LCD.WriteAt(score, SCREENW - 40, 5);
            }
        }

        if (object_visible &&
            abs((object_x + SPRITEW / 2) - (x0 + CHARW / 2)) < CHARW / 2 &&
            abs((object_y + SPRITEH / 2) - (y0 + CHARH / 2)) < CHARH / 2)
        {
            LCD.SetFontColor(LIGHTYELLOW);
            LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);
            object_visible = false;

            if (current_food_index < 4)
            {
                score += 5;
                healthyFoodsEaten++;
            }
            else
            {
                score -= 10; //SCORE INCREMENT CHANGED FOR LEVEL 2
                unhealthyFoodsEaten++;
            }

            LCD.SetFontColor(LIGHTYELLOW);
            LCD.FillRectangle(SCREENW - 40, 5, 40, 20);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Score:", SCREENW - 120, 5);
            LCD.WriteAt(score, SCREENW - 40, 5);
        }

        if (score >= 100)
        {
            levelReached++;
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Food moves faster!"); //instructions for next level (3)
            Sleep(3.0);
            LCD.SetBackgroundColor(LIGHTCORAL);
            LCD.Clear();
            Level3();
            break;
        }
        if (score <= 0)
        {
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            character_image = FEHImage("Gravestone.png");
            character_image.Draw(x0, y0);
            Sleep(1.5);
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.WriteLine("Game Over :(");
            Sleep(3.0);
            ShowStats();
            break;
        }

        if (LCD.Touch(&x, &y))
        {
            LCD.SetFontColor(LIGHTYELLOW);
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            x0 = x - CHARW / 2;
            y0 = y - CHARH / 2;

            if (x0 < 0)
                x0 = 0;
            if (x0 + CHARW > SCREENW)
                x0 = SCREENW - CHARW;
            if (y0 < 0)
                y0 = 0;
            if (y0 + CHARH > SCREENH)
                y0 = SCREENH - CHARH;

            character_image.Draw(x0, y0);
        }

        Sleep(0.01);
    }
}

void Level3()
{
    /*
                            ***********
    refer to Level1(); for most of the documentation. The game functions are mostly the same
                            ***********
    */
    int x, y, x0 = SCREENW / 2, y0 = SCREENH / 2;
    float total_time = TimeNow();
    float current_time = 0, elapsed_time = 0.0;

    int object_x = 0, object_y = -SPRITEH;
    float object_move_time = 0.0;
    bool object_visible = false;
    int current_food_index = 0;
    int score = 50;

    LCD.SetBackgroundColor(LIGHTCORAL); //new background color
    LCD.Clear();

    FEHImage character_image("character2.png");

    character_image.Draw(x0, y0);

    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Score:", SCREENW - 120, 5);
    LCD.WriteAt(score, SCREENW - 40, 5);

    while (1)
    {
        current_time = TimeNow();
        elapsed_time = current_time - total_time;
        total_time = current_time;

        if (score >= 75)
        {
            character_image = FEHImage("character1.png");
        }
        else if (score >= 50)
        {
            character_image = FEHImage("character2.png");
        }
        else if (score >= 40)
        {
            character_image = FEHImage("character3.png");
        }
        else if (score >= 25)
        {
            character_image = FEHImage("character4.png");
        }
        else if (score >= 10)
        {
            character_image = FEHImage("character5.png");
        }
        else
        {
            character_image = FEHImage("character6.png");
        }

        character_image.Draw(x0, y0);

        if (!object_visible)
        {
            object_x = Random.RandInt() % (SCREENW - 60);
            object_y = -SPRITEH;
            current_food_index = Random.RandInt() % 8;
            object_visible = true;
        }

        if (object_visible)
        {
            object_move_time += elapsed_time;
            if (object_move_time >= 0.015) //faster movement for this level
            {
                LCD.SetFontColor(LIGHTCORAL);
                LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);

                object_y += 5;
                object_move_time = 0.0;

                if (object_y <= SCREENH)
                {
                    foods[current_food_index].Draw(object_x, object_y);
                }
                else
                {
                    object_visible = false;
                }

                LCD.SetFontColor(LIGHTCORAL);
                LCD.FillRectangle(SCREENW - 120, 5, 120, 20);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Score:", SCREENW - 120, 5);
                LCD.WriteAt(score, SCREENW - 40, 5);
            }
        }

        if (object_visible &&
            abs((object_x + SPRITEW / 2) - (x0 + CHARW / 2)) < CHARW / 2 &&
            abs((object_y + SPRITEH / 2) - (y0 + CHARH / 2)) < CHARH / 2)
        {
            LCD.SetFontColor(LIGHTCORAL);
            LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);
            object_visible = false;

            if (current_food_index < 4)
            {
                score += 5;
                healthyFoodsEaten++;
            }
            else
            {
                score -= 10;
                unhealthyFoodsEaten++;
            }

            LCD.SetFontColor(LIGHTCORAL);
            LCD.FillRectangle(SCREENW - 40, 5, 40, 20);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Score:", SCREENW - 120, 5);
            LCD.WriteAt(score, SCREENW - 40, 5);
        }
        if (score >= 100)
        {
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("Congrats!"); //finish message but will move this to level 5.
            Sleep(3);
            ShowStats();
            break;
        }
        if (score <= 0)
        {
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            character_image = FEHImage("Gravestone.png");
            character_image.Draw(x0, y0);
            Sleep(1.5);
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.WriteLine("Game Over :(");
            Sleep(3.0);
            ShowStats();
            break;
        }

        if (LCD.Touch(&x, &y))
        {
            LCD.SetFontColor(LIGHTCORAL);
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            x0 = x - CHARW / 2;
            y0 = y - CHARH / 2;

            if (x0 < 0)
                x0 = 0;
            if (x0 + CHARW > SCREENW)
                x0 = SCREENW - CHARW;
            if (y0 < 0)
                y0 = 0;
            if (y0 + CHARH > SCREENH)
                y0 = SCREENH - CHARH;

            character_image.Draw(x0, y0);
        }

        Sleep(0.01);
    }
}
/*
void Level4()
{
    int x, y, x0 = SCREENW / 2, y0 = SCREENH / 2; // Player's position
    float total_time = TimeNow();                 // Time tracking for updates
    float current_time = 0, elapsed_time = 0.0;

    int object_x = 0, object_y = -SPRITEH; // Object starts off-screen at top
    float object_move_time = 0.0;         // Time for moving objects
    bool object_visible = false;           // Is an object currently on screen?

    int current_food_index = 0; // Which food is currently falling
    int score = 50;

    // Array of food images
    FEHImage foods[8] = {
        FEHImage("apple.png"),
        FEHImage("avocado.png"),
        FEHImage("carrot.png"),
        FEHImage("chicken.png"),
        FEHImage("hamburger.png"),
        FEHImage("soda.png"),
        FEHImage("candy.png"),
        FEHImage("cookie.png")};

    LCD.SetBackgroundColor(DARKORANGE); // Change background color to Light Coral
    LCD.Clear();

    FEHImage character_image("character2.png");

    character_image.Draw(x0, y0); // Draw player character

    // Draw initial score
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Score:", SCREENW - 120, 5);
    LCD.WriteAt(score, SCREENW - 40, 5);

    while (1)
    {
        current_time = TimeNow();
        elapsed_time = current_time - total_time;
        total_time = current_time; // Update total elapsed time

        // Change character image based on score (similar to Level 1)
        if (score >= 75)
        {
            character_image = FEHImage("character1.png");
        }
        else if (score >= 50)
        {
            character_image = FEHImage("character2.png");
        }
        else if (score >= 40)
        {
            character_image = FEHImage("character3.png");
        }
        else if (score >= 25)
        {
            character_image = FEHImage("character4.png");
        }
        else if (score >= 10)
        {
            character_image = FEHImage("character5.png");
        }
        else
        {
            character_image = FEHImage("character6.png");
        }

        character_image.Draw(x0, y0);

        // Spawn new object every 2 seconds
        if (!object_visible)
        {
            object_x = Random.RandInt() % (SCREENW - 60); // Random x position
            object_y = -SPRITEH;                          // Start at the top
            current_food_index = Random.RandInt() % 8;    // Random food index
            object_visible = true;                        // Make the food visible
        }

        // Move object down at 5 pixels per 0.015 seconds
        if (object_visible)
        {
            object_move_time += elapsed_time;
            if (object_move_time >= 0.015)
            {
                LCD.SetFontColor(DARKORANGE); // Erase previous object
                LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);

                object_y += 5;          // Move down
                object_move_time = 0.0; // Reset move timer

                // Draw new position
                if (object_y <= SCREENH)
                {
                    foods[current_food_index].Draw(object_x, object_y);
                }
                else
                {
                    object_visible = false; // Food moves off-screen
                }

                // Redraw the score after moving the object
                LCD.SetFontColor(DARKORANGE);                 // Erase the old score text
                LCD.FillRectangle(SCREENW - 120, 5, 120, 20); // Clear the area for score
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Score:", SCREENW - 120, 5);
                LCD.WriteAt(score, SCREENW - 40, 5); // Redraw the score
            }
        }

        // Check for collision between player and food
        if (object_visible &&
            abs((object_x + SPRITEW / 2) - (x0 + CHARW / 2)) < CHARW / 2 &&
            abs((object_y + SPRITEH / 2) - (y0 + CHARH / 2)) < CHARH / 2)
        {
            LCD.SetFontColor(DARKORANGE); // Erase food on collision
            LCD.FillRectangle(object_x, object_y, SPRITEW, SPRITEH);
            object_visible = false; // Food disappears

            // Update score
            if (current_food_index < 4) // Healthy food
            {
                score += 5;
            }
            else // Unhealthy food
            {
                score -= 10; // In Level 2, unhealthy food gives a bigger penalty
            }

            // Update displayed score
            LCD.SetFontColor(DARKORANGE); // Erase previous score
            LCD.FillRectangle(SCREENW - 40, 5, 40, 20);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Score:", SCREENW - 120, 5);
            LCD.WriteAt(score, SCREENW - 40, 5);
        }
        // Transition to Level 3 when score reaches 100
        if (score >= 100)
        {
            LCD.SetBackgroundColor(BLACK);
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("SUDDEN DEATH!");
            Sleep(3.0);
            LCD.SetBackgroundColor(MAROON); // Change background to Dark Orange
            LCD.Clear();
            Level5(); // Start Level 2
            break;    // Exit the Level1 loop to prevent continuous running
        }
        // Handle player movement (similar to Level 1)
        if (LCD.Touch(&x, &y))
        {
            LCD.SetFontColor(DARKORANGE); // Erase previous player position
            LCD.FillRectangle(x0, y0, CHARW, CHARH);
            x0 = x - CHARW / 2; // Update player position
            y0 = y - CHARH / 2;

            // Keep the character within screen bounds
            if (x0 < 0)
                x0 = 0;
            if (x0 + CHARW > SCREENW)
                x0 = SCREENW - CHARW;
            if (y0 < 0)
                y0 = 0;
            if (y0 + CHARH > SCREENH)
                y0 = SCREENH - CHARH;

            character_image.Draw(x0, y0); // Redraw player
        }

        Sleep(0.01); // Prevent high CPU usage
    }
}
*/
void Menu()
{

    int x, y;

    while (1)
    { //always run
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
        while (!LCD.Touch(&x, &y))
        {
        };

        //which button clicked
        if (x > 14 && x < 88 && y > 25 && y < 118)
        { //play
            Level1();
        }
        else if (x > 14 && x < 88 && y > 125 && y < 217)
        { //how 2 play
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteLine("1. Move by clicking!");
            LCD.WriteLine("2. Dodge unhealthy foods!");
            LCD.WriteLine("3. Eat healthy foods!");
            LCD.SetFontColor(RED);
            LCD.WriteAt("Back", 0, 220);

            //if back clicked
            while (1)
            {
                if (LCD.Touch(&x, &y))
                {
                    if (x > 0 && x < 50 && y > 220 && y < 240)
                    {
                        LCD.Clear();
                        Sleep(0.1);
                        break; //back to menu
                    }
                }
            }
        }
        else if (x > 93 && x < 166 && y > 25 && y < 118)
        { //statistics
            ShowStats();
        }
        else if (x > 93 && x < 166 && y > 125 && y < 217)
        { //credits
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
            while (1)
            {
                if (LCD.Touch(&x, &y))
                {
                    if (x > 0 && x < 50 && y > 220 && y < 240)
                    {
                        LCD.Clear();
                        Sleep(0.1);
                        break;// back to menu
                    }
                }
            }
        }
    }
}

int main()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    Menu();

    while (1)
    {
        LCD.Update(); //never quit
    }
    return 0;
}