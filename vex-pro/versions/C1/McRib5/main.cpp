#include "robot-config.h"

vex::competition    Competition;

int buttonPressed = 0;

bool red;
bool front;
int auton = 0;

int promise = 4;

int getFlywheelRPM(){
    return Flywheel.velocity(vex::velocityUnits::rpm)*-1;
}

void controllerPrint(){
    while (1){
        Controller.Screen.clearScreen();
        Controller.Screen.print(getFlywheelRPM());
        vex::task::sleep(250);
    }
}

void controllerVibrate(){
    while(1){
        if (getFlywheelRPM() > 170){
            Controller.rumble("-");
        }
    }
}

void move(bool reverse, double inches, double percent, bool asynchronous) {
    
    double degPerInch = 27;//28.64 is actual
    double rotationGoal = degPerInch*inches;
    
    bool left;
    bool right;
    if (reverse) {
        left = true;
        right = false;
    } else {
        left = false;
        right = true;
    }
    
    LeftFront.setReversed(left);
    LeftBack.setReversed(left);
    RightFront.setReversed(right);
    RightBack.setReversed(right);
    
    LeftFront.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    LeftBack.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    RightFront.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    RightBack.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, asynchronous); //Execute all commands at the same time
    
    LeftFront.stop();
    LeftBack.stop();
    RightFront.stop();
    RightBack.stop();    
}

void rotate(bool reverse, double inches, double percent, bool asynchronous) {
    
    double degPerInch = 27;//28.64 is actual
    double rotationGoal = degPerInch*inches;
    
    LeftFront.setReversed(reverse);
    LeftBack.setReversed(reverse);
    RightFront.setReversed(reverse);
    RightBack.setReversed(reverse);
    
    LeftFront.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    LeftBack.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    RightFront.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    RightBack.rotateFor(rotationGoal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, asynchronous); //Execute all commands at the same time
    
    LeftFront.stop();
    LeftBack.stop();
    RightFront.stop();
    RightBack.stop();    
}

int shoot() {
    int goal = Brain.timer(vex::timeUnits::sec)+promise;
    while(1) {
        if (getFlywheelRPM() > 175) {
            Intake.setReversed(false);
            Intake.rotateFor(1360, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
            return -1;
        }   
        if(Brain.timer(vex::timeUnits::sec) > goal) {
            return 1;
        }
    }
}

void auton0( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton1( void ) {
    //RED FRONT AUTON
    Flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);

    //ALIGN WITH WALL AND MOVE BACK INTO EXPANSION ZONE TO LOWER ARM
    move(false, 56, 100, true);
    move(true, 56, 100, true);
    Arm.rotateFor(-210, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    Arm.stop(vex::brakeType::coast);

    //MOVE INTO POSITION, SHOOT FLAG, AND STOP FLYWHEEL
    move(false, 36, 100, true);
    shoot();
    Flywheel.stop(vex::brakeType::coast);

    //TURN 90 DEGREES, MOVE FORWARD, AND FLIP CAP
    rotate(false, 17, 100, true);  
    move(true, 10, 100, true);
    move(false, 38, 100, true);

    Arm.rotateFor(50, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    Arm.stop(vex::brakeType::coast);
    move(true, 11, 100, true);

    //TURN TOWARDS PLATFORM AND PARK
    rotate(false, 51, 100, true);
    move(true, 48, 100, true);
    
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton2( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton3( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton4( void ) {
    Flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
        
    move(false, 24, 100, true);
    shoot();

    Flywheel.stop(vex::brakeType::coast);
    move(true, 24, 100, true);
    
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton5( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton6( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton7( void ) {
    //BLUE FRONT AUTON
    Flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);

    //ALIGN WITH WALL AND MOVE BACK INTO EXPANSION ZONE TO LOWER ARM
    move(false, 56, 100, true);
    move(true, 56, 100, true);
    Arm.rotateFor(-210, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    Arm.stop(vex::brakeType::coast);

    //MOVE INTO POSITION, SHOOT FLAG, AND STOP FLYWHEEL
    move(false, 36, 100, true);
    shoot();
    Flywheel.stop(vex::brakeType::coast);

    //TURN 90 DEGREES, MOVE FORWARD, AND FLIP CAP
    rotate(true, 17, 100, true);  
    move(true, 10, 100, true);
    move(false, 38, 100, true);

    Arm.rotateFor(50, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    Arm.stop(vex::brakeType::coast);
    move(true, 11, 100, true);

    //TURN TOWARDS PLATFORM AND PARK
    rotate(true, 51, 100, true);
    move(true, 48, 100, true);
    
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton8( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton9( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton10( void ) {
    Flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);

    move(false, 24, 100, true);
    shoot();

    Flywheel.stop(vex::brakeType::coast);
    move(true, 24, 100, true);
    
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton11( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void auton12( void ) {
    Brain.Screen.clearScreen(vex::color::white);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Lol.");
}

void screens(int screen) {
    if (screen == 1) {
        auton = 0;
        
        Brain.Screen.clearScreen(vex::color::white);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(2);
        
        yDisplacement = 0;
        xDisplacement = 120;
        x = xDisplacement;
        y = yDisplacement;
        
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
        
        int balls[16][2][2] = {
            {{65, 120}, {60, 60}},
            {{65, 160}, {60, 60}},
            {{175, 120}, {60, 60}},
            {{175, 160}, {60, 60}},
            {{100, 100}, {60, 60}},
            {{140, 100}, {60, 60}},
            {{100, 180}, {60, 60}},
            {{140, 180}, {60, 60}},
            {{55, 55}, {60, 60}},
            {{65, 65}, {60, 60}},
            {{175, 65}, {60, 60}},
            {{185, 55}, {60, 60}},
            {{100, 214}, {60, 60}},
            {{100, 226}, {60, 60}},
            {{140, 214}, {60, 60}},
            {{140, 226}, {60, 60}}
        };
        
        for(int i = 0; i < 8; i++) {
            Brain.Screen.setPenColor(caps[i][1][0]);
            Brain.Screen.drawCircle(caps[i][0][0]+xDisplacement, caps[i][0][1]+yDisplacement, capsRadiusPX, caps[i][1][1]);
        }
        
        for(int i = 0; i < 4; i++) {
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
        
        for(int i = 0; i < 3; i++) {
            int length = platforms[i][0][1] - platforms[i][0][0];
            int width = platforms[i][1][1] - platforms[i][1][0];

            Brain.Screen.setPenColor(platforms[i][2][0]);
            Brain.Screen.drawRectangle(platforms[i][0][0]+xDisplacement, platforms[i][1][0]+yDisplacement, width, length, platforms[i][2][1]);
        }
        
        for(int i = 0; i < 16; i++) {
            Brain.Screen.setPenColor(balls[i][1][0]);
            Brain.Screen.drawCircle(balls[i][0][0]+xDisplacement, balls[i][0][1]+yDisplacement, ballsRadiusPX, balls[i][1][1]);
        }
        
        buttonPressed = 0;
        while (buttonPressed == 0) {
            for(int i = 0; i < 4; i++) {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]+xDisplacement) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1]+xDisplacement)) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0]+yDisplacement && 
                     Brain.Screen.yPosition() < buttons[i][1][1]+yDisplacement)) &&
                   Brain.Screen.pressing()) {
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
        auton = 0;
        
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
        
        int buttons[4][2][2] = {
            {{0, 160}, {25, 180}},
            {{160, 320}, {25, 180}},
            {{320, 480}, {25, 180}},
            {{400, 480}, {180, 240}}
        };
        
        for(int i = 0; i < 4; i++) {
            int width = buttons[i][0][1] - buttons[i][0][0];
            int length = buttons[i][1][1] - buttons[i][1][0];

            Brain.Screen.setPenColor(vex::color::black);
            Brain.Screen.drawRectangle(buttons[i][0][0], buttons[i][1][0], width, length, color+(i*30));
            if(i == 3) {
                Brain.Screen.drawRectangle(buttons[i][0][0], buttons[i][1][0], width, length, vex::color::white);
            }
        }
        
        Brain.Screen.setCursor(11, 43);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFillColor(vex::color::white);
        Brain.Screen.print("Back");
        
        buttonPressed = 0;
        while (buttonPressed == 0) {
            for(int i = 0; i < 4; i++) {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1])) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0] && 
                     Brain.Screen.yPosition() < buttons[i][1][1])) &&
                   Brain.Screen.pressing()) {
                    vex::task::sleep(1000); //or add wait until function...
                    buttonPressed = i+1;
                }
            }
            vex::task::sleep(25);
        }
        
        if (buttonPressed == 4) {
            screens(1);
        } else if (red) {
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
        
        int buttons[1][2][2] = {
            {{400, 480}, {180, 240}}
        };
        
        for(int i = 0; i < 1; i++) {
            int width = buttons[i][0][1] - buttons[i][0][0];
            int length = buttons[i][1][1] - buttons[i][1][0];

            Brain.Screen.setPenColor(vex::color::black);
            Brain.Screen.drawRectangle(buttons[i][0][0], buttons[i][1][0], width, length, vex::color::white);
        }
        
        Brain.Screen.setCursor(11, 43);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFillColor(vex::color::white);
        Brain.Screen.print("Back");
        
        buttonPressed = 0;
        while (buttonPressed == 0) {
            for(int i = 0; i < 1; i++) {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1])) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0] && 
                     Brain.Screen.yPosition() < buttons[i][1][1])) &&
                   Brain.Screen.pressing()) {
                    vex::task::sleep(1000); //or add wait until function...
                    screens(2);
                    buttonPressed = i+1;
                }
            }
            vex::task::sleep(25);
        }
    } else if (screen == 4) {
        Brain.Screen.clearScreen(vex::color::white);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(2);
        Brain.Screen.setFont(vex::fontType::mono20);
        Brain.Screen.setFillColor(vex::color::white);
    }
}

void pre_auton( void ) {
  screens(1);
};

void autonomous( void ) {
    if (auton == 0) {
        auton0();
    } else if (auton == 1) {
        auton1();
    } else if (auton == 2) {
        auton2();
    } else if (auton == 3) {
        auton3();
    } else if (auton == 4) {
        auton4();
    } else if (auton == 5) {
        auton5();
    } else if (auton == 6) {
        auton6();
    } else if (auton == 7) {
        auton7();
    } else if (auton == 8) {
        auton8();
    } else if (auton == 9) {
        auton9();
    } else if (auton == 10) {
        auton10();
    } else if (auton == 11) {
        auton11();
    } else if (auton == 12) {
        auton12();
    }
}

void usercontrol( void ) {
    LeftFront.setReversed(true);
    LeftBack.setReversed(true);
    RightFront.setReversed(true);
    RightBack.setReversed(true);
    Arm.setReversed(false);
    vex::thread controllerPrint_t(controllerPrint);
    vex::thread controllerVibrate_t(controllerVibrate);

    while(true) {

       //Drive Control
      LeftFront.spin(vex::directionType::rev, Controller.Axis3.value()*0.75, vex::velocityUnits::pct);
      LeftBack.spin(vex::directionType::rev, Controller.Axis3.value()*0.75, vex::velocityUnits::pct);
      RightFront.spin(vex::directionType::fwd, Controller.Axis2.value()*0.75, vex::velocityUnits::pct);
      RightBack.spin(vex::directionType::fwd, Controller.Axis2.value()*0.75, vex::velocityUnits::pct);

      //Flywheel Control
      if(Controller.ButtonA.pressing()) { //If button A is pressed...
          Flywheel.spin(vex::directionType::rev, flySpeedPCT, vex::velocityUnits::pct);
      }
      else {
          if (getFlywheelRPM() < (flySpeedPCT/2))
          {
              Flywheel.spin(vex::directionType::rev, flySpeedPCT/4, vex::velocityUnits::pct);
          } else {
              Flywheel.stop(vex::brakeType::coast);
          }
      }

      //Intake Control
      if(Controller.ButtonL1.pressing()) {
          Intake.spin(vex::directionType::rev, intakeSpeedPCT, vex::velocityUnits::pct);
      } else if(Controller.ButtonL2.pressing()) {
          Intake.spin(vex::directionType::fwd, intakeSpeedPCT, vex::velocityUnits::pct);
      } else {
          Intake.stop(vex::brakeType::brake);       
      }

      //Arm Control
      if(Controller.ButtonR1.pressing()) {
          Arm.spin(vex::directionType::fwd, armSpeedPCT, vex::velocityUnits::pct);
      } else if(Controller.ButtonR2.pressing()) {
          Arm.spin(vex::directionType::rev, armSpeedPCT, vex::velocityUnits::pct);
      } else {
          Arm.stop(vex::brakeType::brake);       
      }

      //Claw Control
      if(Controller.ButtonB.pressing()) {
          Claw.spin(vex::directionType::fwd, armSpeedPCT, vex::velocityUnits::pct);
      } else {
          Claw.stop(vex::brakeType::brake);
      }

      vex::task::sleep(20);
    }    
}

int main() {
    pre_auton();
    
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    while(1) {
        vex::task::sleep(25);
    }
}
