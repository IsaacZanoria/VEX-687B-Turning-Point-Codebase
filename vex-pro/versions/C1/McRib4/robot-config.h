vex::brain Brain;

vex::motor  LeftFront(vex::PORT18, vex::gearSetting::ratio18_1, true);
vex::motor  LeftBack(vex::PORT17, vex::gearSetting::ratio18_1, true);
vex::motor  RightFront(vex::PORT20, vex::gearSetting::ratio18_1, true);
vex::motor  RightBack(vex::PORT19, vex::gearSetting::ratio18_1, true);
vex::motor  Flywheel(vex::PORT2, vex::gearSetting::ratio18_1, true);
vex::motor  Arm(vex::PORT3, vex::gearSetting::ratio18_1, true);
vex::motor  Claw(vex::PORT4, vex::gearSetting::ratio18_1, true);
vex::motor  Intake(vex::PORT1, vex::gearSetting::ratio18_1, true);

vex::controller Controller(vex::controllerType::primary);

int flySpeedPCT = 100;
int intakeSpeedPCT = 100;
int armSpeedPCT = 50;