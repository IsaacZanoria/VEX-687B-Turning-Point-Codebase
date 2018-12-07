#include "robot-config.h"

int buttonPressed = 0;

bool red;
bool front;
int auton = 0;

void screens(int screen) {
    if (screen == 1) {
        Brain.Screen.clearScreen(vex::color::white);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(2);
        
        for (int i = 0; i < tiles; i++) {
            x = xDisplacement;
            for (int j = 0; j < tiles; j++) {
                Brain.Screen.drawRectangle(x, y, tile_px, tile_px, "#838383");
                x += tile_px;
            }
            y += tile_px;
        }
        
        int buttons[4][3][2] = {
            {{0, 40}, {80 , 120}, {0, 0}},          //1: Top Left Red
            {{0, 40}, {160, 200}, {0, 0}},          //2: Bottom Left Red
            {{200, 240}, {80, 120}, {240, 240}},    //3: Top Right Blue
            {{200, 240}, {160, 200}, {240, 240}}    //4: Bottom Right Blue
        };
        
        int platforms[3][3][2] = {
            {{65, 105}, {120, 160}, {0, 0}}, //red
            {{135, 175}, {120, 160}, {240, 240}}, //blue
            {{100, 140}, {120, 160}, {60, 60}} //yellow
        };
        
        int caps[8][2][2] = {
            {{60, 60}, {240, 240}},
            {{100, 100}, {0, 0}},
            {{100, 180}, {0, 0}},
            {{100, 220}, {240, 240}},
            {{180, 60}, {0, 0}},
            {{140, 100}, {240, 240}},
            {{140, 180}, {240, 240}},
            {{140, 220}, {0, 0}}
        };
        
        for(int i = 0; i < 8; i++)
        {
            Brain.Screen.setPenColor(caps[i][1][0]);
            Brain.Screen.drawCircle(caps[i][0][0]+xDisplacement, caps[i][0][1]+yDisplacement, capsRadiusPX, caps[i][1][1]);
        }
        
        for(int i = 0; i < 4; i++)
        {
            int length = buttons[i][0][1] - buttons[i][0][0];
            int width = buttons[i][1][1] - buttons[i][1][0];

            Brain.Screen.setPenColor(buttons[i][2][0]);
            Brain.Screen.drawRectangle(buttons[i][0][0]+xDisplacement, buttons[i][1][0]+yDisplacement, width, length, buttons[i][2][1]);
        }
        
        //Expansion Zone
        Brain.Screen.setPenColor(vex::color::white);
        Brain.Screen.setPenWidth(2);
        Brain.Screen.drawLine(0+xDisplacement, 120+yDisplacement, 40+xDisplacement, 120+yDisplacement);
        Brain.Screen.drawLine(200+xDisplacement, 120+yDisplacement, 240+xDisplacement, 120+yDisplacement);
        Brain.Screen.drawLine(40+xDisplacement, 120+yDisplacement, 40+xDisplacement, 200+yDisplacement);
        Brain.Screen.drawLine(200+xDisplacement, 120+yDisplacement, 200+xDisplacement, 200+yDisplacement);
        
        Brain.Screen.drawLine(40+xDisplacement, 200+yDisplacement, 200+xDisplacement, 200+yDisplacement);
        Brain.Screen.drawLine(120+xDisplacement, 240+yDisplacement, 120+xDisplacement, 0+yDisplacement);
        
        //Flag Pole
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(7);
        Brain.Screen.drawLine(0+xDisplacement, 0+yDisplacement, 0+xDisplacement, 40+yDisplacement);
        Brain.Screen.drawLine(0+xDisplacement, 0+yDisplacement, 240+xDisplacement, 0+yDisplacement);
        Brain.Screen.drawLine(240+xDisplacement, 0+yDisplacement, 240+xDisplacement, 40+yDisplacement);
        Brain.Screen.drawLine(40+xDisplacement, 0+yDisplacement, 40+xDisplacement, 20+yDisplacement);
        Brain.Screen.drawLine(120+xDisplacement, 0+yDisplacement, 120+xDisplacement, 20+yDisplacement);
        Brain.Screen.drawLine(200+xDisplacement, 0+yDisplacement, 200+xDisplacement, 20+yDisplacement);
        
        //Flags
        Brain.Screen.setPenWidth(3);
        Brain.Screen.setPenColor(vex::color::red);
        Brain.Screen.drawLine(120+xDisplacement, 20+yDisplacement, 134+xDisplacement, 6+yDisplacement);
        Brain.Screen.drawLine(200+xDisplacement, 20+yDisplacement, 220+xDisplacement, 20+yDisplacement);
        
        Brain.Screen.setPenColor(vex::color::blue);
        Brain.Screen.drawLine(40+xDisplacement, 20+yDisplacement, 20+xDisplacement, 20+yDisplacement);
        Brain.Screen.drawLine(120+xDisplacement, 20+yDisplacement, 106+xDisplacement, 6+yDisplacement);
        
        for(int i = 0; i < 3; i++)
        {
            int length = platforms[i][0][1] - platforms[i][0][0];
            int width = platforms[i][1][1] - platforms[i][1][0];

            Brain.Screen.setPenColor(platforms[i][2][0]);
            Brain.Screen.drawRectangle(platforms[i][0][0]+xDisplacement, platforms[i][1][0]+yDisplacement, width, length, platforms[i][2][1]);
        }
        
        buttonPressed = 0;
        while (buttonPressed == 0) {
            for(int i = 0; i < 4; i++)
            {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]+xDisplacement) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1]+xDisplacement)) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0]+yDisplacement && 
                     Brain.Screen.yPosition() < buttons[i][1][1]+yDisplacement)) &&
                   Brain.Screen.pressing())
                {
                    vex::task::sleep(1000); //or add wait until function...
                    buttonPressed = i+1;
                }
            }
            vex::task::sleep(25);
        }
        
        if (buttonPressed == 1) {
            red = true;
            front = true;
        } else if (buttonPressed == 2) {
            red = true;
            front = false;
        } else if (buttonPressed == 3) {
            red = false;
            front = true;
        } else if (buttonPressed == 4) {
            red = false;
            front = false;
        }
        screens(2);
        
    } else if (screen == 2) {
        Brain.Screen.clearScreen(vex::color::white);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(2);

        int color;
        if (red) {
            color = 0;
        } else {
            color = 240;
        }
        
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.setPenColor(color);
        Brain.Screen.setFillColor(vex::color::white);
        if (red) {
            if (front) {
                Brain.Screen.print("Red Front Selected", true);
            } else {
                Brain.Screen.print("Red Back Selected", true);
            }
        } else {
            if (front) {
                Brain.Screen.print("Blue Front Selected", true);
            } else {
                Brain.Screen.print("Blue Back Selected", true);
            }
        }
        
        int buttons[3][2][2] = {
            {{0, 160}, {25,240}},
            {{160, 320}, {25,240}},
            {{320, 480}, {25,240}}
        };
        
        for(int i = 0; i < 3; i++)
        {
            int length = buttons[i][0][1] - buttons[i][0][0];
            int width = buttons[i][1][1] - buttons[i][1][0];

            Brain.Screen.setPenColor(vex::color::black);
            Brain.Screen.drawRectangle(buttons[i][0][0], buttons[i][1][0], width, length, color+(i*30));
        }
        
        buttonPressed = 0;
        while (buttonPressed == 0) {
            for(int i = 0; i < 3; i++)
            {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1])) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0] && 
                     Brain.Screen.yPosition() < buttons[i][1][1])) &&
                   Brain.Screen.pressing())
                {
                    vex::task::sleep(1000); //or add wait until function...
                    buttonPressed = i+1;
                }
            }
            vex::task::sleep(25);
        }
        Brain.Screen.setPenColor(vex::color::black);
        if (red) {
            if (front) {
                if (buttonPressed == 1) {
                    auton = 1;
                } else if (buttonPressed == 2) {
                    auton = 2;
                } else if (buttonPressed == 3) {
                    auton = 3;
                }
            } else {
                if (buttonPressed == 1) {
                    auton = 4;
                } else if (buttonPressed == 2) {
                    auton = 5;
                } else if (buttonPressed == 3) {
                    auton = 6;
                }
            }
        } else  {
            if (front) {
                if (buttonPressed == 1) {
                    auton = 7;
                } else if (buttonPressed == 2) {
                    auton = 8;
                } else if (buttonPressed == 3) {
                    auton = 9;
                }
            } else {
                if (buttonPressed == 1) {
                    auton = 10;
                } else if (buttonPressed == 2) {
                    auton = 11;
                } else if (buttonPressed == 3) {
                    auton = 12;
                }
            }
        }
        
        screens(3);
    } else if (screen == 3) {
        Brain.Screen.clearScreen(vex::color::white);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(2);
        Brain.Screen.setFont(vex::fontType::mono20);
        
        //TODO: Explain what each auton does and select auton in preauton.
        
        if (auton == 0) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Do nothing.");
        } else if (auton == 1) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 1.");
        } else if (auton == 2) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 2.");
        } else if (auton == 3) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 3.");
        } else if (auton == 4) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 4.");
        } else if (auton == 5) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 5.");
        } else if (auton == 6) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 6.");
        } else if (auton == 7) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 7.");
        } else if (auton == 8) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 8.");
        } else if (auton == 9) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 9.");
        } else if (auton == 10) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 10.");
        } else if (auton == 11) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 11.");
        } else if (auton == 12) {
            Brain.Screen.setCursor(1, 1);
            Brain.Screen.print("Auton 12.");
        }
    }
}

int main() {
    screens(1);
}