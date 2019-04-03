// VEX V5 C++ Project with Competition Template
#include "vex.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

//#region config_globals
vex::brain Brain;
vex::controller Controller(vex::controllerType::primary);
vex::pot pot_flipper(Brain.ThreeWirePort.C);
vex::pot pot_arm(Brain.ThreeWirePort.A);
vex::pot pot_catapult(Brain.ThreeWirePort.B);
vex::motor motor_left_front(vex::PORT19, vex::gearSetting::ratio18_1, false);
vex::motor motor_left_back(vex::PORT20, vex::gearSetting::ratio18_1, false);
vex::motor motor_right_front(vex::PORT17, vex::gearSetting::ratio18_1, true);
vex::motor motor_right_back(vex::PORT16, vex::gearSetting::ratio18_1, true);
vex::motor motor_intake(vex::PORT15, vex::gearSetting::ratio18_1, true);
vex::motor motor_flipper(vex::PORT9, vex::gearSetting::ratio18_1, false);
vex::motor motor_arm(vex::PORT10, vex::gearSetting::ratio18_1, true);
vex::motor motor_catapult(vex::PORT18, vex::gearSetting::ratio18_1, false);
//#endregion config_globals

// Creates a competition object that allows access to Competition methods.
vex::competition Competition;

std::string side = "none";
std::string position = "none";
int screen_id = 1;
bool first_run = true;
bool auton = false;

void move(bool reverse, double inches, double percent, bool asynchronous) {
    
    double degreeswheel_per_inch = 42.97; // 27 //28.64 is actual (need to multiply by gear ratio 18:12)
    double rotation_goal = degreeswheel_per_inch*inches;
    
    bool left;
    bool right;
    if (reverse) {
        left = true;
        right = false;
    } else {
        left = false;
        right = true;
    }
    
    motor_left_front.setReversed(left);
    motor_left_back.setReversed(left);
    motor_right_front.setReversed(right);
    motor_right_back.setReversed(right);
    
    motor_left_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    motor_left_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    motor_right_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    motor_right_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, asynchronous); //Execute all commands at the same time
    
    motor_left_front.stop(vex::brakeType::brake);
    motor_left_back.stop(vex::brakeType::brake);
    motor_right_front.stop(vex::brakeType::brake);
    motor_right_back.stop(vex::brakeType::brake);
}

void rotate(bool reverse, double degrees, double percent, bool asynchronous) {
    
    double degreeswheel_per_inch = 42.97;//number of degrees each wheel motor needs to turn to move 1 inch (need to multiply by gear ratio of 18:12) (42.971834634811741)
    double degreesrobot_per_inch = 0.164/1.5; //number of inches robot needs to move to rotate 1 degree (0.163624617374468)
    double rotation_goal = degreeswheel_per_inch*degreesrobot_per_inch*degrees;
    
    motor_left_front.setReversed(reverse);
    motor_left_back.setReversed(reverse);
    motor_right_front.setReversed(reverse);
    motor_right_back.setReversed(reverse);
    
    motor_left_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    motor_left_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    motor_right_front.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, false);
    motor_right_back.rotateFor(rotation_goal, vex::rotationUnits::deg, percent, vex::velocityUnits::pct, asynchronous); //Execute all commands at the same time
}

void setup_catapult() {
    while (pot_catapult.value(vex::analogUnits::range12bit) == 0) {
        vex::this_thread::sleep_for(50);
        //wait
    }
    
    while (pot_catapult.value(vex::analogUnits::range12bit) > 275 || pot_catapult.value(vex::analogUnits::range12bit) < 75) {//Launch angle
        motor_catapult.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    }
    motor_catapult.stop(vex::brakeType::brake);
}

bool available_catapult = true;
void launch_catapult() {
    if (available_catapult) {
        available_catapult = false;
        motor_catapult.rotateFor(vex::directionType::fwd, 600, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
        vex::this_thread::sleep_for(1500);
        setup_catapult();
        motor_catapult.stop(vex::brakeType::brake);
        available_catapult = true;
    }
}

bool available_flipper = true;
void rotate_flipper() {
    if (available_flipper) {
        available_flipper = false;
        motor_flipper.rotateFor(vex::directionType::fwd, 450, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
        available_flipper = true;
        motor_flipper.stop(vex::brakeType::brake);
    }
}

//each tile is 24' x 24'
void auton_1() {
    //NONE
}

void auton_2() {
    //RED NEAR
    
    //drive forward and hit the wall
    move(false, 24, 100, true);
    
    
    //setup and shoot flag
    setup_catapult();
    launch_catapult();
    
    //knock red cap (@bryan need to test if robot will pass middle line)
    move(true, 18, 100, true);
    rotate(true, 90, 100, true);
    move(false, 5, 50, true);
    
    //setup and park on low platform
    rotate(true, 90, 100, true);
    move(true, 15, 50, true);
    move(false, 24, 100, true);
}

void auton_3() {
    //RED FAR
    
    //shoot flag
    motor_arm.rotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
    motor_intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    
    //knock cone
    rotate(true, 90, 100, true);
    move(false, 18, 100, true);
    
    //park on low platform
    rotate(false, 90, 100, true);
    move(true, 15, 100, true);
    move(false, 24, 100, true);
    
    motor_intake.stop(vex::brakeType::brake);
}

void auton_4() {
    //BLUE NEAR (@ bryan mirror auton_2)
    //drive forward and hit the wall
    move(false, 50, 100, true);
    
    //drive backwards (more)
    move(true, 80, 100, true);
    
    motor_arm.rotateFor(vex::directionType::rev, 3500, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
    
    //drive forwards (less)
    move(false, 45, 100, true);
    
    //setup and shoot
    setup_catapult();
    launch_catapult();
    launch_catapult();
}

void auton_5() {
    //BLUE FAR (@bryan mirror auton_3)
    motor_arm.rotateFor(vex::directionType::rev, 1000, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
    
    motor_intake.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    
    move(false, 24, 100, true);
    
    move(true, 24, 100, true);
    
    motor_intake.stop(vex::brakeType::brake);
}

void autonomous() {
    auton = true;
    first_run = false;
    // Place autonomous code here
    if (side == "none" && position == "none") {
        vex::thread auton_thread_1 (auton_1);
    } else if (side == "red" && position == "near") {
        vex::thread auton_thread_2 (auton_2);
    } else if (side == "red" && position == "far") {
        vex::thread auton_thread_3 (auton_3);
    } else if (side == "blue" && position == "near") {
        vex::thread auton_thread_4 (auton_4);
    } else if (side == "blue" && position == "far") {
        vex::thread auton_thread_5 (auton_5);
    }
    screen_id = 6;
}

class Button {
private:
    int get_pressed_xpos() {
        if (!Brain.Screen.pressing()) {
            //std::cout << "Last X: " << Brain.Screen.xPosition() << std::endl;
            return Brain.Screen.xPosition();
        }
        return -1;
    }
    
    int get_pressed_ypos() {
        if (!Brain.Screen.pressing()) {
            //std::cout << "Last Y: " << Brain.Screen.yPosition() << std::endl;
            return Brain.Screen.yPosition();
        }
        return -1;
    }
    
    int get_pressing_xpos() {
        if (Brain.Screen.pressing()) {
            //std::cout << "Last X: " << Brain.Screen.xPosition() << std::endl;
            return Brain.Screen.xPosition();
        }
        return -1;
    }
    
    int get_pressing_ypos() {
        if (Brain.Screen.pressing()) {
            //std::cout << "Last Y: " << Brain.Screen.yPosition() << std::endl;
            return Brain.Screen.yPosition();
        }
        return -1;
    }
public:
    int xpos;
    int ypos;
    int width;
    int height;

    void set(int xpos, int ypos, int width, int height) {
        this -> xpos = xpos;
        this -> ypos = ypos;
        this -> width = width;
        this -> height = height;
    }

    /*void draw() {
        Brain.Screen.setPenWidth(border_width);
        Brain.Screen.setPenColor(border_color);
        Brain.Screen.drawRectangle(xpos, ypos, width, height, fill_color);
    }*/ 
    
    bool pressed() {
        // if get_pressed_(y/x)pos is within button boundries, return true, else false;
        int rect_w_max = xpos + width;
        int rect_w_min = xpos;
        int rect_h_max = ypos + height;
        int rect_h_min = ypos;
        if (get_pressed_xpos() > rect_w_min &&
            get_pressed_xpos() < rect_w_max &&
            get_pressed_ypos() > rect_h_min &&
            get_pressed_ypos() < rect_h_max) {
            // change x y pos
            return true;
        }
        
        return false;
    }
    
    bool pressing() {
        // if get_pressed_(y/x)pos is within button boundries, return true, else false;
        int rect_w_max = xpos + width;
        int rect_w_min = xpos;
        int rect_h_max = ypos + height;
        int rect_h_min = ypos;
        if (get_pressing_xpos() > rect_w_min &&
            get_pressing_xpos() < rect_w_max &&
            get_pressing_ypos() > rect_h_min &&
            get_pressing_ypos() < rect_h_max) {
            // change x y pos
            return true;
        }
        
        return false;
    }
};
 
int max_drop_size = 5;
int min_drop_size = 2;
int drop_speed = 1;
double size_speed = 0.55;
class Drop {
public:
    double x_pos = rand() % 480;
    double y_pos = 0;
    int size = rand() % (max_drop_size - min_drop_size + 1) + min_drop_size;
 
    void update()
    {
        y_pos += size * size_speed / (double)drop_speed;
    }
};

class Snowflake {
public:
    std::vector<Drop> drops;
    
    void update_drops(unsigned milliseconds_elapsed) {
        //Iterate and update all drop in drops
        for (auto &drop : drops) {
            drop.update();
        }
 
        //Erase drop that fall over 240px off screen
        drops.erase(std::remove_if(drops.begin(), drops.end(), [](const Drop &drop) {
            return drop.y_pos > 240;
        }), drops.end());
    }
 
    void generate_drops(unsigned milliseconds_elapsed) {
        //std::cout << "MSEC ELAPSED: "<< milliseconds_elapsed << "\tDROPS: " << drops.size() << std::endl;
        for (int i = 0; i <  rand() % 2; i++) {
            //Push and add object Drop in vector drops
            drops.push_back(Drop());
        }
    }
    
    void draw(unsigned milliseconds_elapsed) {
        
    }
 
    std::vector<Drop> get_drops() {
        return drops;
    }
};

class Screen {
private:
    static constexpr int milliseconds_per_frame = 17; //17: 60fps || 33: 30fps
    
    Snowflake snowflake;
    
    int hue = 0;
    int last_tick_time = 0;
 
    void loop_color(unsigned milliseconds_elapsed) {
        if (hue >= 359) { //359 for smooth color transition
            hue = 0;
        }
        hue ++;
    }
 
    void screen_1(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
 
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
        
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        Brain.Screen.setFont(vex::fontType::prop60);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("687B") / 2, 80, false, "687B");
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("McRib v2") / 2, 110, false, "McRib v2");
        
        Button b_auton;
        b_auton.set(480 / 2 - 80 / 2, 240 / 2 - 40 / 2 + 40, 80, 40);
        if (b_auton.pressing()) {
            while (b_auton.pressing()) {
                
            }
            if (b_auton.pressed()) {
                screen_id = 2;
            }
        }
        Button b_stats;
        b_stats.set(480 / 2 - 80 / 2 - 117, 240 / 2 - 40 / 2 + 40, 80, 40);
        if (b_stats.pressing()) {
            while (b_stats.pressing()) {
                
            }
            if (b_stats.pressed()) {
                screen_id = 3;
            }
        }
        Brain.Screen.setPenWidth(2);
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2, 240 / 2 - 40 / 2 + 40, 80, 40, "#d6d5db");
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2 - 117, 240 / 2 - 40 / 2 + 40, 80, 40, "#d6d5db");
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2 + 117, 240 / 2 - 40 / 2 + 40, 80, 40, "#d6d5db");
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Auton") / 2, 240 / 2 + 45, false, "Auton");
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Stats") / 2 - 117, 240 / 2 + 45, false, "Stats");
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("WIP") / 2 + 117, 240 / 2 + 45, false, "WIP");
    
        Brain.Screen.render(true);
    }
    
    void screen_2(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
       
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
       
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(60, 70, false, "Auton Selection");
        Brain.Screen.drawLine(50, 85, 430, 85);
        Brain.Screen.drawLine(50, 185, 430, 185);
        Brain.Screen.drawLine(240, 185, 240, 215);
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 207, false , "Side:");
        if (side == "red") {
            Brain.Screen.setPenColor(vex::color::red);
        } else if (side == "blue") {
            Brain.Screen.setPenColor(vex::color::blue);
        } else {
            Brain.Screen.setPenColor(vex::color::black);
        }
        Brain.Screen.printAt(110, 207, false, side.c_str());
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(250, 207, false, "Position: %s%%", position.c_str());
        
        Brain.Screen.setPenColor(vex::color::red);
        Brain.Screen.drawRectangle(70, 103, 100, 60, vex::color::red);
        Brain.Screen.setPenColor(vex::color::white);
        Brain.Screen.setFont(vex::fontType::prop30);
        Brain.Screen.printAt(95, 143, false, "Red");
        
        Brain.Screen.setPenColor("#d6d5db");
        Brain.Screen.drawRectangle(480 / 2 - 100 / 2, 103, 100, 60, "#d6d5db");
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("None") / 2, 143, false, "None");
        
        Brain.Screen.setPenColor(vex::color::blue);
        Brain.Screen.drawRectangle(310, 103, 100, 60, vex::color::blue);
        Brain.Screen.setPenColor(vex::color::white);
        Brain.Screen.printAt(333, 143, false, "Blue");
        
        Button b_red;
        b_red.set(70, 103, 100, 60);
        if (b_red.pressing()) {
            while (b_red.pressing()) {
                
            }
            if (b_red.pressed()) {
                side = "red";
                screen_id = 4;
            }
        }
        Button b_none;
        b_none.set(480 / 2 - 100 / 2, 103, 100, 60);
        if (b_none.pressing()) {
            while (b_none.pressing()) {
                
            }
            if (b_none.pressed()) {
                side = "none";
                position = "none";
                screen_id = 5;
            }
        }
        Button b_blue;
        b_blue.set(310, 103, 100, 60);
        if (b_blue.pressing()) {
            while (b_blue.pressing()) {
                
            }
            if (b_blue.pressed()) {
                side = "blue";
                screen_id = 4;
            }
        }
        Button b_back;
        b_back.set(480 / 2 - 80 / 2 + 140, 35, 80, 40);
        if (b_back.pressing()) {
            while (b_back.pressing()) {
                
            }
            if (b_back.pressed()) {
                screen_id = 1;
            }
        }
        
        Brain.Screen.setPenWidth(2);
        Brain.Screen.setPenColor(hue);
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2 + 140, 35, 80, 40, "#d6d5db");
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Back") / 2 + 140, 60, false, "Back");
        
        Brain.Screen.render(true);
    }
    
    void screen_3(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
       
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
        
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        Brain.Screen.setFont(vex::fontType::prop40);
        Brain.Screen.printAt(60, 70, false, "Stats");
        Brain.Screen.drawLine(50, 85, 430, 85);
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(60, 110, false, "Battery Capacity: %d%%%d%%", Brain.Battery.capacity(), "%");
        Brain.Screen.printAt(60, 130, false, "Battery Temperature: %d%%%d%%", Brain.Battery.temperature(), "%");
        
        Button b_back;
        b_back.set(480 / 2 - 80 / 2 + 140, 35, 80, 40);
        if (b_back.pressing()) {
            while (b_back.pressing()) {
                
            }
            if (b_back.pressed()) {
                screen_id = 1;
            }
        }
        
        Brain.Screen.setPenWidth(2);
        Brain.Screen.setPenColor(hue);
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2 + 140, 35, 80, 40, "#d6d5db");
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Back") / 2 + 140, 60, false, "Back");
        
        Brain.Screen.render(true);
    }
    
    void screen_4(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
       
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
        
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(60, 70, false, "Auton Selection");
        Brain.Screen.drawLine(50, 85, 430, 85);
        Brain.Screen.drawLine(50, 185, 430, 185);
        Brain.Screen.drawLine(240, 185, 240, 215);
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 207, false , "Side:");
        if (side == "red") {
            Brain.Screen.setPenColor(vex::color::red);
        } else if (side == "blue") {
            Brain.Screen.setPenColor(vex::color::blue);
        } else {
            Brain.Screen.setPenColor(vex::color::black);
        }
        Brain.Screen.printAt(110, 207, false, side.c_str());
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(250, 207, false, "Position: %s", position.c_str());
        
        Brain.Screen.setPenColor("#d6d5db");
        Brain.Screen.drawRectangle(130, 103, 100, 60, "#d6d5db");
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop30);
        Brain.Screen.printAt(150, 143, false, "Near");
        
        Brain.Screen.setPenColor("#d6d5db");
        Brain.Screen.drawRectangle(250, 103, 100, 60, "#d6d5db");
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop30);
        Brain.Screen.printAt(280, 143, false, "Far");
        
        Button b_near;
        b_near.set(130, 103, 100, 60);
        if (b_near.pressing()) {
            while (b_near.pressing()) {
                
            }
            if (b_near.pressed()) {
                position  = "near";
                screen_id = 5;
            }
        }
        Button b_far;
        b_far.set(250, 103, 100, 60);
        if (b_far.pressing()) {
            while (b_far.pressing()) {
                
            }
            if (b_far.pressed()) {
                position = "far";
                screen_id = 5;
            }
        }
        Button b_back;
        b_back.set(480 / 2 - 80 / 2 + 140, 35, 80, 40);
        if (b_back.pressing()) {
            while (b_back.pressing()) {
                
            }
            if (b_back.pressed()) {
                screen_id = 2;
            }
        }
        
        Brain.Screen.setPenWidth(2);
        Brain.Screen.setPenColor(hue);
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2 + 140, 35, 80, 40, "#d6d5db");
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Back") / 2 + 140, 60, false, "Back");
        
        Brain.Screen.render(true);
    }
    
    void screen_5(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
       
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
        
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(60, 70, false, "Auton Selection");
        Brain.Screen.drawLine(50, 85, 430, 85);
        Brain.Screen.drawLine(50, 185, 430, 185);
        Brain.Screen.drawLine(240, 185, 240, 215);
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 207, false , "Side:");
        if (side == "red") {
            Brain.Screen.setPenColor(vex::color::red);
        } else if (side == "blue") {
            Brain.Screen.setPenColor(vex::color::blue);
        } else {
            Brain.Screen.setPenColor(vex::color::black);
        }
        Brain.Screen.printAt(110, 207, false, side.c_str());
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(250, 207, false, "Position: %s", position.c_str());
        
        Button b_run;
        b_run.set(310, 103, 100, 60);
        if (b_run.pressing()) {
            while (b_run.pressing()) {
                
            }
            if (b_run.pressed()) {
                autonomous();
            }
        }
        Button b_back;
        b_back.set(480 / 2 - 80 / 2 + 140, 35, 80, 40);
        if (b_back.pressing()) {
            while (b_back.pressing()) {
                autonomous();
                auton = true;
            }
            if (b_back.pressed()) {
                screen_id = 2;
            }
        }
        
        Brain.Screen.setPenColor("#d6d5db");
        Brain.Screen.drawRectangle(310, 103, 100, 60, "#d6d5db");
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop30);
        Brain.Screen.printAt(335, 143, false, "Run");
        
        Brain.Screen.setPenWidth(2);
        Brain.Screen.setPenColor(hue);
        Brain.Screen.drawRectangle(480 / 2 - 80 / 2 + 140, 35, 80, 40, "#d6d5db");
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Back") / 2 + 140, 60, false, "Back");
        
        Brain.Screen.render(true);
    }
    
    const char* loading[4] = {"", "\\", "l", "/"};
    int index = 0;
    int total_elapsed = 0;
    int now = 0;
    void screen_6(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
       
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
        
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        
        Brain.Screen.setFont(vex::fontType::prop40);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Autonomous") / 2, 60, false, "Autonomous");
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Running") / 2, 100, false, "Running");
        
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 - 100, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 - 50, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 + 50, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 + 100, 140, false, loading[index]);
        total_elapsed++;
        if (total_elapsed >= 5) {
            index++;
            if (index >= 4) {
                index = 0;
            }
            total_elapsed = 0;
        }
        
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 170, false , "Side:");
        if (side == "red") {
            Brain.Screen.setPenColor(vex::color::red);
        } else if (side == "blue") {
            Brain.Screen.setPenColor(vex::color::blue);
        } else {
            Brain.Screen.setPenColor(vex::color::black);
        }
        Brain.Screen.printAt(140, 170, false, side.c_str());
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 190, false, "Position: %s", position.c_str());
        
        if (now == 0) {
            now = Brain.Timer.time();
        }
        Brain.Screen.printAt(200, 170, false, "Time Elapsed: %ds", (Brain.Timer.time() - now) / 1000);
        
        Brain.Screen.render(true);
    }
    
    int now2 = 0;
    void screen_7(unsigned milliseconds_elapsed) {
        Brain.Screen.clearScreen();
       
        snowflake.update_drops(milliseconds_elapsed);
        //todo: throttle following
        snowflake.generate_drops(milliseconds_elapsed);
        loop_color(milliseconds_elapsed);
        Brain.Screen.setPenWidth(1);
        Brain.Screen.setPenColor(hue);
        for (auto &drop : snowflake.get_drops()) {
            Brain.Screen.drawRectangle(drop.x_pos, drop.y_pos, drop.size, drop.size, hue);
        }
        
        Brain.Screen.setPenWidth(5);
        Brain.Screen.drawRectangle(50, 25, 380, 190, vex::color::white);
        
        Brain.Screen.setFont(vex::fontType::prop40);
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Driver") / 2, 60, false, "Driver");
        Brain.Screen.printAt(480 / 2 - Brain.Screen.getStringWidth("Control") / 2, 100, false, "Control");
        
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 - 100, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 - 50, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 + 50, 140, false, loading[index]);
        Brain.Screen.printAt(480 / 2 -Brain.Screen.getStringWidth(loading[index]) / 2 + 100, 140, false, loading[index]);
        total_elapsed++;
        if (total_elapsed >= 5) {
            index++;
            if (index >= 4) {
                index = 0;
            }
            total_elapsed = 0;
        }
        
        Brain.Screen.setFont(vex::fontType::prop20);
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 170, false , "Side:");
        if (side == "red") {
            Brain.Screen.setPenColor(vex::color::red);
        } else if (side == "blue") {
            Brain.Screen.setPenColor(vex::color::blue);
        } else {
            Brain.Screen.setPenColor(vex::color::black);
        }
        Brain.Screen.printAt(140, 170, false, side.c_str());
        
        Brain.Screen.setPenColor(vex::color::black);
        Brain.Screen.printAt(60, 190, false, "Position: %s", position.c_str());
        
        if (now2 == 0) {
            now2 = Brain.Timer.time();
        }
        Brain.Screen.printAt(200, 170, false, "Time Elapsed: %ds", (Brain.Timer.time() - now2) / 1000);
        
        Brain.Screen.render(true);
    }
    
public:
    Screen() {
        last_tick_time = Brain.Timer.time(vex::timeUnits::msec);
    }
    
    void tick() {
        int now = Brain.Timer.time(vex::timeUnits::msec);
        int elapsed = now - last_tick_time;
        last_tick_time = now;        

        if (screen_id == 1) {
            screen_1(elapsed);
        } else if (screen_id == 2) {
            screen_2(elapsed);
        } else if (screen_id == 3) {
            screen_3(elapsed);
        } else if (screen_id == 4) {
            screen_4(elapsed);
        } else if (screen_id == 5) {
            screen_5(elapsed);
        } else if (screen_id == 6) {
            screen_6(elapsed);
        } else if (screen_id == 7) {
            screen_7(elapsed);
        }
        
        if (elapsed < milliseconds_per_frame) {
            //Calculate time per frame after time elapsed running functions
            //std::cout << (milliseconds_per_frame - elapsed) << "\tELAPSED: " << elapsed << std::endl;
            vex::this_thread::sleep_for(milliseconds_per_frame - elapsed);
        }
    }
};

bool screen_bool = true;
void run_screen() {
    Screen screen;
    while (screen_bool) {
        screen.tick();
        
    }
}

void stop_screen() {
    screen_bool = false;
}

void pre_auton() {
    // All activities that occur before competition start
    // Example: setting initial positions
    vex::thread screen_thread(run_screen);
}

double drive_speed = 1.00;
double intake_speed = 1.00;
double flipper_speed = 1.00;
double arm_speed = 1.00;
double catapult_speed = 1.00;
void drivercontrol() {
    auton = false;
    if (!first_run) {
        screen_id = 7;
    }
    // Place drive control code here, inside the loop
    while (!auton) {
        //std::cout << "driver running \t" << Brain.Timer.time() << std::endl; 
        // This is the main loop for the driver control.
        // Each time through the loop you should update motor
        // movements based on input from the controller.

        motor_left_front.spin(vex::directionType::fwd, Controller.Axis3.position(vex::percentUnits::pct), vex::velocityUnits::pct);
        motor_left_back.spin(vex::directionType::fwd, Controller.Axis3.position(vex::percentUnits::pct), vex::velocityUnits::pct);
        motor_right_front.spin(vex::directionType::fwd, Controller.Axis2.position(vex::percentUnits::pct), vex::velocityUnits::pct);
        motor_right_back.spin(vex::directionType::fwd, Controller.Axis2.position(vex::percentUnits::pct), vex::velocityUnits::pct);
    
        //Intake
        if(Controller.ButtonL1.pressing()) {
            motor_intake.spin(vex::directionType::fwd, 100 * intake_speed, vex::velocityUnits::pct);
        } else if(Controller.ButtonL2.pressing()) {
            motor_intake.spin(vex::directionType::rev, 100 * intake_speed, vex::velocityUnits::pct);
        } else {
            motor_intake.stop(vex::brakeType::brake);       
        }
        
        if (!((Controller.ButtonR1.pressing() && Controller.ButtonY.pressing()) || (Controller.ButtonR2.pressing() && Controller.ButtonY.pressing()))) {
            //Flipper
            if (Controller.ButtonY.pressing()) {
                vex::thread launch_flipper_thread(rotate_flipper);
            }
            
            //Arm
            if(Controller.ButtonR1.pressing()) {
                motor_arm.spin(vex::directionType::fwd, 100 * arm_speed, vex::velocityUnits::pct);
                motor_flipper.spin(vex::directionType::fwd, 17, vex::velocityUnits::pct);
            } else if(Controller.ButtonR2.pressing()) {
                motor_arm.spin(vex::directionType::rev, 100 * arm_speed, vex::velocityUnits::pct);
                motor_flipper.spin(vex::directionType::rev, 17, vex::velocityUnits::pct);
            } else {
                motor_arm.stop(vex::brakeType::brake);
                if (available_catapult && available_flipper) {
                    motor_flipper.stop(vex::brakeType::brake);
                }
            }
        }
        
        if (Controller.ButtonLeft.pressing()) {
            motor_flipper.rotateFor(vex::directionType::fwd, 50, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
        }
        
        if (Controller.ButtonDown.pressing()) {
            motor_flipper.rotateFor(vex::directionType::rev, 50, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
        }
        
        //Catapult    
        if (Controller.ButtonA.pressing()) {
            vex::thread launch_catapult_thread (launch_catapult);
        }
        
        vex::this_thread::sleep_for(20);
    }
}

void test() {
    while (1) {
        std::cout << "---------------------------" << Brain.Timer.time() << std::endl;
        std::cout << "Catapult:" << pot_catapult.value(vex::analogUnits::range12bit) << std::endl;
        vex::this_thread::sleep_for(100);
        //140
    }
}

int main() {
    // Do not adjust the lines below
    vex::thread test_thread(test);
    
    // Set up (but don't start) callbacks for autonomous and driver control periods.
    Competition.autonomous(autonomous);
    Competition.drivercontrol(drivercontrol);

    // Run the pre-autonomous function.
    pre_auton();

    // Robot Mesh Studio runtime continues to run until all threads and
    // competition callbacks are finished.
}
