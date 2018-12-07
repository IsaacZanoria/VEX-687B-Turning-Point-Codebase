#include "robot-config.h"

int button(int x, int y) {
    Brain.Screen.setPenWidth(2);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setFillColor(vex::color::white);
    
    int button[4][3][2] = {
        {{0, 40}, {80 , 120}, {0, 0}},
        {{0, 40}, {160, 200}, {0, 0}},
        {{200, 240}, {80, 120}, {240, 240}},
        {{200, 240}, {160, 200}, {240, 240}}
    };
    
    for(int i = 0; i < 4; i++)
    {
        int length = button[i][0][1] - button[i][0][0];
        int width = button[i][1][1] - button[i][1][0];

        Brain.Screen.setPenColor(button[i][2][0]);
        Brain.Screen.drawRectangle(button[i][0][0]+xDisplacement, button[i][1][0]+yDisplacement, width, length, button[i][2][1]);

        if(((x > button[i][0][0]) && (x < button[i][0][1])) && ((y > button[i][1][0] && y < button[i][1][1])))
        {
            return i+1;
        }
    }
    return 0;
}

int main() {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setOrigin(0,0);
    Brain.Screen.setPenWidth(2);
    Brain.Screen.setPenColor(vex::color::black);
    
    for (int i = 0; i < tiles; i++) {
        x = xDisplacement;
        for (int j = 0; j < tiles; j++) {
            Brain.Screen.drawRectangle(x, y, tile_px, tile_px, vex::color::white);
            x += tile_px;
        }
        y += tile_px;
    }
    
    while (1) {
        button(Brain.Screen.xPosition(), Brain.Screen.yPosition());
    }
    
}