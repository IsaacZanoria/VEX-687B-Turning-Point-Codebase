#include "robot-config.h"

int button_pressed = 0;

bool red;
bool front;
int auton = 0;

int promise = 6;

int get_flywheel_rpm(){
    return flywheel.velocity(vex::velocityUnits::rpm)*-1;
}

void controller_print(){
    while (1){
        controller.Screen.clearScreen();
        controller.Screen.print(get_flywheel_rpm());
        vex::task::sleep(250);
    }
}

void controller_vibrate(){
    while(1){
        if (get_flywheel_rpm() > 170) {
            controller.rumble("-");
        }
    }
}

void move(bool reverse, double inches, double percent, bool asynchronous) {
    
    double degrees_per_inch = 27;//28.64 is actual
    double rotation_goal = degrees_per_inch*inches;
    
    bool left;
    bool right;
    if (reverse) {
        left = true;
        right = false;
    } else {
        left = false;
        right = true;
    }
    
    left_front.setReversed(left);
    left_back.setReversed(left);
    right_front.setReversed(right);
    right_back.setReversed(right);
    
    left_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    left_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    right_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    right_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, asynchronous); //Execute all commands at the same time
    
    left_front.stop();
    left_back.stop();
    right_front.stop();
    right_back.stop();    
}

void rotate(bool reverse, double inches, double percent, bool asynchronous) {
    
    double degrees_per_inch = 27;//28.64 is actual
    double rotation_goal = degrees_per_inch*inches;
    
    left_front.setReversed(reverse);
    left_back.setReversed(reverse);
    right_front.setReversed(reverse);
    right_back.setReversed(reverse);
    
    left_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    left_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    right_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    right_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, asynchronous); //Execute all commands at the same time
    
    left_front.stop();
    left_back.stop();
    right_front.stop();
    right_back.stop();    
}

int shoot() {
    int goal = Brain.timer(vex::timeUnits::sec)+promise;
    while(1) {
        if (get_flywheel_rpm() > 175) {
            ball_intake.setReversed(false);
            ball_intake.rotateFor(1360, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
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
    flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);

    //ALIGN WITH WALL AND MOVE BACK INTO EXPANSION ZONE TO LOWER ARM
    move(false, 56, 100, true);
    move(true, 56, 100, true);
    arm.rotateFor(-210, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    arm.stop(vex::brakeType::coast);

    //MOVE INTO POSITION, SHOOT FLAG, AND STOP FLYWHEEL
    move(false, 36, 100, true);
    shoot();
    flywheel.stop(vex::brakeType::coast);

    //TURN 90 DEGREES, MOVE FORWARD, AND FLIP CAP
    rotate(false, 17, 100, true);  
    move(true, 10, 100, true);
    move(false, 38, 100, true);

    arm.rotateFor(50, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    arm.stop(vex::brakeType::coast);
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
    flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
        
    move(false, 24, 100, true);
    shoot();

    flywheel.stop(vex::brakeType::coast);
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
    flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);

    //ALIGN WITH WALL AND MOVE BACK INTO EXPANSION ZONE TO LOWER ARM
    move(false, 56, 100, true);
    move(true, 56, 100, true);
    arm.rotateFor(-210, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    arm.stop(vex::brakeType::coast);

    //MOVE INTO POSITION, SHOOT FLAG, AND STOP FLYWHEEL
    move(false, 36, 100, true);
    shoot();
    flywheel.stop(vex::brakeType::coast);

    //TURN 90 DEGREES, MOVE FORWARD, AND FLIP CAP
    rotate(true, 17, 100, true);  
    move(true, 10, 100, true);
    move(false, 38, 100, true);

    arm.rotateFor(50, vex::rotationUnits::deg, 80, vex::velocityUnits::pct);
    arm.stop(vex::brakeType::coast);
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
    flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);

    move(false, 24, 100, true);
    shoot();

    flywheel.stop(vex::brakeType::coast);
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
        
        y_displacement = 0;
        x_displacement = 120;
        x = x_displacement;
        y = y_displacement;
        
        for (int i = 0; i < tiles; i++) {
            x = x_displacement;
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
            Brain.Screen.drawCircle(caps[i][0][0]+x_displacement, caps[i][0][1]+y_displacement, capsRadiusPX, caps[i][1][1]);
        }
        
        for(int i = 0; i < 4; i++) {
            int length = buttons[i][0][1] - buttons[i][0][0];
            int width = buttons[i][1][1] - buttons[i][1][0];

            Brain.Screen.setPenColor(buttons[i][2][0]);
            Brain.Screen.drawRectangle(buttons[i][0][0]+x_displacement, buttons[i][1][0]+y_displacement, width, length, buttons[i][2][1]);
        }
        
        //Expansion Zone
        Brain.Screen.setPenColor(vex::color::white);
        Brain.Screen.setPenWidth(2);
        Brain.Screen.drawLine(0+x_displacement, 120+y_displacement, 40+x_displacement, 120+y_displacement);
        Brain.Screen.drawLine(200+x_displacement, 120+y_displacement, 240+x_displacement, 120+y_displacement);
        Brain.Screen.drawLine(40+x_displacement, 120+y_displacement, 40+x_displacement, 200+y_displacement);
        Brain.Screen.drawLine(200+x_displacement, 120+y_displacement, 200+x_displacement, 200+y_displacement);
        
        Brain.Screen.drawLine(40+x_displacement, 200+y_displacement, 200+x_displacement, 200+y_displacement);
        Brain.Screen.drawLine(120+x_displacement, 240+y_displacement, 120+x_displacement, 0+y_displacement);
        
        //Flag Pole
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setPenWidth(7);
        Brain.Screen.drawLine(0+x_displacement, 0+y_displacement, 0+x_displacement, 40+y_displacement);
        Brain.Screen.drawLine(0+x_displacement, 0+y_displacement, 240+x_displacement, 0+y_displacement);
        Brain.Screen.drawLine(240+x_displacement, 0+y_displacement, 240+x_displacement, 40+y_displacement);
        Brain.Screen.drawLine(40+x_displacement, 0+y_displacement, 40+x_displacement, 20+y_displacement);
        Brain.Screen.drawLine(120+x_displacement, 0+y_displacement, 120+x_displacement, 20+y_displacement);
        Brain.Screen.drawLine(200+x_displacement, 0+y_displacement, 200+x_displacement, 20+y_displacement);
        
        //Flags
        Brain.Screen.setPenWidth(3);
        Brain.Screen.setPenColor(vex::color::red);
        Brain.Screen.drawLine(120+x_displacement, 20+y_displacement, 134+x_displacement, 6+y_displacement);
        Brain.Screen.drawLine(200+x_displacement, 20+y_displacement, 220+x_displacement, 20+y_displacement);
        
        Brain.Screen.setPenColor(vex::color::blue);
        Brain.Screen.drawLine(40+x_displacement, 20+y_displacement, 20+x_displacement, 20+y_displacement);
        Brain.Screen.drawLine(120+x_displacement, 20+y_displacement, 106+x_displacement, 6+y_displacement);
        
        for(int i = 0; i < 3; i++) {
            int length = platforms[i][0][1] - platforms[i][0][0];
            int width = platforms[i][1][1] - platforms[i][1][0];

            Brain.Screen.setPenColor(platforms[i][2][0]);
            Brain.Screen.drawRectangle(platforms[i][0][0]+x_displacement, platforms[i][1][0]+y_displacement, width, length, platforms[i][2][1]);
        }
        
        for(int i = 0; i < 16; i++) {
            Brain.Screen.setPenColor(balls[i][1][0]);
            Brain.Screen.drawCircle(balls[i][0][0]+x_displacement, balls[i][0][1]+y_displacement, ballsRadiusPX, balls[i][1][1]);
        }
        
        button_pressed = 0;
        while (button_pressed == 0) {
            for(int i = 0; i < 4; i++) {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]+x_displacement) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1]+x_displacement)) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0]+y_displacement && 
                     Brain.Screen.yPosition() < buttons[i][1][1]+y_displacement)) &&
                   Brain.Screen.pressing()) {
                    vex::task::sleep(1000); //or add wait until function...
                    button_pressed = i+1;
                }
            }
            vex::task::sleep(25);
        }
        
        if (button_pressed == 1) {
            red = true;
            front = true;
        } else if (button_pressed == 2) {
            red = true;
            front = false;
        } else if (button_pressed == 3) {
            red = false;
            front = true;
        } else if (button_pressed == 4) {
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
        
        button_pressed = 0;
        while (button_pressed == 0) {
            for(int i = 0; i < 4; i++) {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1])) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0] && 
                     Brain.Screen.yPosition() < buttons[i][1][1])) &&
                   Brain.Screen.pressing()) {
                    vex::task::sleep(1000); //or add wait until function...
                    button_pressed = i+1;
                }
            }
            vex::task::sleep(25);
        }
        
        if (button_pressed == 4) {
            screens(1);
        } else if (red) {
            if (front) {
                if (button_pressed == 1) {
                    auton = 1;
                } else if (button_pressed == 2) {
                    auton = 2;
                } else if (button_pressed == 3) {
                    auton = 3;
                }
            } else {
                if (button_pressed == 1) {
                    auton = 4;
                } else if (button_pressed == 2) {
                    auton = 5;
                } else if (button_pressed == 3) {
                    auton = 6;
                }
            }
        } else  {
            if (front) {
                if (button_pressed == 1) {
                    auton = 7;
                } else if (button_pressed == 2) {
                    auton = 8;
                } else if (button_pressed == 3) {
                    auton = 9;
                }
            } else {
                if (button_pressed == 1) {
                    auton = 10;
                } else if (button_pressed == 2) {
                    auton = 11;
                } else if (button_pressed == 3) {
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
        
        button_pressed = 0;
        while (button_pressed == 0) {
            for(int i = 0; i < 1; i++) {
                if(((Brain.Screen.xPosition() > buttons[i][0][0]) && 
                    (Brain.Screen.xPosition() < buttons[i][0][1])) && 
                   ((Brain.Screen.yPosition() > buttons[i][1][0] && 
                     Brain.Screen.yPosition() < buttons[i][1][1])) &&
                   Brain.Screen.pressing()) {
                    vex::task::sleep(1000); //or add wait until function...
                    screens(2);
                    button_pressed = i+1;
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
    left_front.setReversed(true);
    left_back.setReversed(true);
    right_front.setReversed(true);
    right_back.setReversed(true);
    arm.setReversed(false);
    vex::thread controller_print_t(controller_print);
    vex::thread controller_vibrate_t(controller_vibrate);
    
    while(true) {

      //Drive Control
      left_front.spin(vex::directionType::rev, controller.Axis3.value()*0.75, vex::velocityUnits::pct);
      left_back.spin(vex::directionType::rev, controller.Axis3.value()*0.75, vex::velocityUnits::pct);
      right_front.spin(vex::directionType::fwd, controller.Axis2.value()*0.75, vex::velocityUnits::pct);
      right_back.spin(vex::directionType::fwd, controller.Axis2.value()*0.75, vex::velocityUnits::pct);

      //Flywheel Control
      if(controller.ButtonA.pressing()) { //If button A is pressed...
          flywheel.spin(vex::directionType::rev, flywheel_speed_percentage, vex::velocityUnits::pct);
      }
      else {
          if (get_flywheel_rpm() < flywheel_rest_speed_threshold_percentage*2)
          {
              flywheel.spin(vex::directionType::rev, flywheel_rest_speed_threshold_percentage/2, vex::velocityUnits::pct);
          } else {
              flywheel.stop(vex::brakeType::coast);
          }
      }

      //Ball Intake Control
      if(controller.ButtonL1.pressing()) {
          ball_intake.spin(vex::directionType::rev, ball_intake_speed_percentage, vex::velocityUnits::pct);
      } else if(controller.ButtonL2.pressing()) {
          ball_intake.spin(vex::directionType::fwd, ball_intake_speed_percentage, vex::velocityUnits::pct);
      } else {
          ball_intake.stop(vex::brakeType::brake);       
      }

      //Arm Control
      if(controller.ButtonR1.pressing()) {
          arm.spin(vex::directionType::fwd, arm_speed_percentage, vex::velocityUnits::pct);
      } else if(controller.ButtonR2.pressing()) {
          arm.spin(vex::directionType::rev, arm_speed_percentage, vex::velocityUnits::pct);
      } else {
          arm.stop(vex::brakeType::brake);       
      }

      //Cap Intake Control
      if(controller.ButtonB.pressing()) {
          cap_intake.spin(vex::directionType::fwd, arm_speed_percentage, vex::velocityUnits::pct);
      } else {
          cap_intake.stop(vex::brakeType::brake);
      }

      vex::task::sleep(20);
    }    
}

int main() {
    pre_auton();
    
    competition.autonomous( autonomous );
    competition.drivercontrol( usercontrol );
    
    while(1) {
        vex::task::sleep(25);
    }
}