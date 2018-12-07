#include "robot-config.h"

/*+++++++++++++++++++++++++++++++++++++++++++++| Notes |++++++++++++++++++++++++++++++++++++++++++++++
Tank Control 
This program instructs your robot to use remote control values to move the robot. 

Robot Configuration:
[Smart Port]    [Name]              [Type]              [Description]           [Reversed]
Motor Port 1    LeftFront           V5 Smart Motor      Left Front motor        false
Motor Port 2    RightBack           V5 Smart Motor      Right Back motor        true
Motor Port 3    Flywheel            V5 Smart Motor      Flywheel motor          false
Motor Port 4    Arm                 V5 Smart Motor      Arm motor               false
Motor Port 7    Claw                V5 Smart Motor      Rotate motor            false
Motor Port 8    Intake              V5 Smart Motor      Intake motor            false
Motor Port 9    LeftBack            V5 Smart Motor      Left Back motor         false
Motor Port 10   RightFront          V5 Smart Motor      Right Front motor       true



----------------------------------------------------------------------------------------------------*/          

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VCS VEX V5                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/

//Creates a competition object that allows access to Competition methods.
vex::competition    Competition;

/*---------------------------------------------------------------------------*/
/*                          Function Definitions                             */
/*---------------------------------------------------------------------------*/
int state = 0;
int auton = -1;
int side = -1;
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

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */ 
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

int button(int x, int y) {
    Brain.Screen.setPenWidth(6);
    Brain.Screen.setPenColor(vex::color::black);
    Brain.Screen.setFillColor(vex::color::white);
    if (state == 0) {
        int button[3][3][2] = {
            {{0, 158}, {0, 250}, {"", ""}}, {{158, 317}, {0, 250}, {"", ""}}, {{317, 475}, {0, 250}, {"", ""}}
        };
        for(int i = 0; i < 3; i++)
        {
            int length = button[i][0][1] - button[i][0][0];
            int width = button[i][1][1] - button[i][1][0];
            
            Brain.Screen.setPenColor(button[i][2][0]);
            Brain.Screen.setFillColor(button[i][2][0]);
            Brain.Screen.drawRectangle(button[i][0][0], button[i][1][0], width, length);
            
            if(((x > button[i][0][0]) && (x < button[i][0][1])) && ((y > button[i][1][0] && y < button[i][1][1])))
            {
                return i+1;
            }
        }
    }
    return 0;
}

/*void autonPrint(char* autonMessage, char* penColorSetting, char* fillColorSetting){
    Brain.Screen.setFont(vex::fontType::mono60);
    Brain.Screen.setPenColor(penColorSetting);
    Brain.Screen.setFillColor(fillColorSetting);
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(0, 40, autonMessage);
    Brain.Screen.render();
}*/

void pre_auton( void ) {
    state = 0;
    
    while(state == 0) {     
        int select = button(Brain.Screen.xPosition(), Brain.Screen.yPosition());
        //Controller.Screen.clearScreen();
        //Controller.Screen.print(select); Logging
        if (select == 1) {
            auton = 0;
            //autonPrint("Static Auton Selected", "#FFFFFF", "#00FF00");
            Brain.Screen.setFont(vex::fontType::mono40);
            Brain.Screen.setPenColor("#FFFFFF");
            Brain.Screen.setFillColor("#00FF00");
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(0, 40, "Static Auton Selected");
            Brain.Screen.render();
            break;
        } else if (select == 2) {
            auton = 1;
            side = 0;
            //autonPrint("Red Front Auton Selected", "#FFFFFF", "#FF0000");
            Brain.Screen.setFont(vex::fontType::mono40);
            Brain.Screen.setPenColor("#FFFFFF");
            Brain.Screen.setFillColor("#FF0000");
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(0, 40, "Red Front Auton Selected");
            Brain.Screen.render();
            break;
        } else if (select == 3) {
            auton = 2;
            side = 1;
            //autonPrint("Blue Front Auton Selected", "#FFFFFF", "#0000FF");
            Brain.Screen.setFont(vex::fontType::mono40);
            Brain.Screen.setPenColor("#FFFFFF");
            Brain.Screen.setFillColor("#0000FF");
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(0, 40, "Blue Front Auton Selected");
            Brain.Screen.render();
            break;
        }
        Brain.Screen.render();
        vex::task::sleep(500);
    };
    
    LeftFront.setStopping(vex::brakeType::brake);
    LeftBack.setStopping(vex::brakeType::brake);
    RightFront.setStopping(vex::brakeType::brake);
    RightBack.setStopping(vex::brakeType::brake);
    
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
    vex::thread controllerPrint_t(controllerPrint);
    vex::thread controllerVibrate_t(controllerVibrate);
    Controller.Screen.clearScreen();
    if (auton == 1) {
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
        
        
    } else if (auton == 2) {
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
        
    } else if (auton == 3) {
        //RED BACK AUTON
        Flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
        
        move(false, 24, 100, true);
        shoot();
        
        Flywheel.stop(vex::brakeType::coast);
        move(true, 24, 100, true);
    } else if (auton == 4) {
        //BLUE BACK AUTON
        Flywheel.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
        
        move(false, 24, 100, true);
        shoot();
        
        Flywheel.stop(vex::brakeType::coast);
        move(true, 24, 100, true);
    }
    controllerPrint_t.interrupt();
    controllerVibrate_t.interrupt();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol( void ) {
    LeftFront.setReversed(true);
    LeftBack.setReversed(true);
    RightFront.setReversed(true);
    RightBack.setReversed(true);
    Arm.setReversed(false);
    vex::thread controllerPrint_t(controllerPrint);
    vex::thread controllerVibrate_t(controllerVibrate);
  // User control code here, inside the loop
    
  if (side == -1) {
      Brain.Screen.setFont(vex::fontType::mono40);
      Brain.Screen.setPenColor("#FFFFFF");
      Brain.Screen.setFillColor("#00FF00");
      Brain.Screen.clearScreen();
      Brain.Screen.printAt(0, 40, "YEET!");
      Brain.Screen.render();
  } else if (side == 0) {
      Brain.Screen.setFont(vex::fontType::mono40);
      Brain.Screen.setPenColor("#FFFFFF");
      Brain.Screen.setFillColor("#FF0000");
      Brain.Screen.clearScreen();
      Brain.Screen.printAt(0, 40, "GO RED!");
      Brain.Screen.render();
  } else if (side == 1) {
      Brain.Screen.setFont(vex::fontType::mono40);
      Brain.Screen.setPenColor("#FFFFFF");
      Brain.Screen.setFillColor("#0000FF");
      Brain.Screen.clearScreen();
      Brain.Screen.printAt(0, 40, "GO BLUE!");
      Brain.Screen.render();
  }
    
  while(true) {

      //Drive Control
      //Set the left and right motor to spin forward using the controller Axis values as the velocity value.
      LeftFront.spin(vex::directionType::rev, Controller.Axis3.value()*0.75, vex::velocityUnits::pct);
      LeftBack.spin(vex::directionType::rev, Controller.Axis3.value()*0.75, vex::velocityUnits::pct);
      RightFront.spin(vex::directionType::fwd, Controller.Axis2.value()*0.75, vex::velocityUnits::pct);
      RightBack.spin(vex::directionType::fwd, Controller.Axis2.value()*0.75, vex::velocityUnits::pct);

      //Flywheel Control
      if(Controller.ButtonA.pressing()) { //If button A is pressed...
          //...Launch ball.
          Flywheel.spin(vex::directionType::rev, flySpeedPCT, vex::velocityUnits::pct);
      }
      else { //If the button A is not pressed...
          ///...Slow the flywheel.
          if (getFlywheelRPM() < (flySpeedPCT/2))
          {
              Flywheel.spin(vex::directionType::rev, flySpeedPCT/4, vex::velocityUnits::pct);
          } else {
              Flywheel.stop(vex::brakeType::coast);
          }
      }

      //Intake Control
      if(Controller.ButtonL1.pressing()) { //If button L1 is pressed...
          //...Lift ball.
          Intake.spin(vex::directionType::rev, intakeSpeedPCT, vex::velocityUnits::pct);
      }
      else if(Controller.ButtonL2.pressing()) { //If button L2 is pressed...
          //...Drop ball.
          Intake.spin(vex::directionType::fwd, intakeSpeedPCT, vex::velocityUnits::pct);
      }
      else
      { //If button L1 or L2 are not pressed...
          //...Stop the intake.
          Intake.stop(vex::brakeType::brake);       
      }

      //Arm Control
      if(Controller.ButtonR1.pressing()) { //If button R1 is pressed...
          //...Lift cap up.
          Arm.spin(vex::directionType::fwd, armSpeedPCT, vex::velocityUnits::pct);
      }
      else if(Controller.ButtonR2.pressing()) { //If button R2 is pressed...
          //...Lift cap down.
          Arm.spin(vex::directionType::rev, armSpeedPCT, vex::velocityUnits::pct);
      }
      else
      { //If button R1 or R2 are not pressed...
          //...Stop the claw motor. 
          Arm.stop(vex::brakeType::brake);       
      }

      //Claw Control
      if(Controller.ButtonB.pressing()) { //If button B is pressed...
          //...Flip cap.
          //Encoder.resetRotation();
          Claw.spin(vex::directionType::fwd, armSpeedPCT, vex::velocityUnits::pct);
          //Encoder.rotation(vex::rotationUnits::180);
      }
      else
      { //If button B is not pressed...
          ///...Stop.
          Claw.stop(vex::brakeType::brake);
      }

      vex::task::sleep(20); //Sleep the task for a short amount of time to prevent wasted resources.
    }    
}

//
// Main will set up the competition functions and callbacks.
//

int main() {
    //Run the pre-autonomous function.
    pre_auton();
    
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    //Prevent main from exiting with an infinite loop.
    vex::task::sleep(25);
}
