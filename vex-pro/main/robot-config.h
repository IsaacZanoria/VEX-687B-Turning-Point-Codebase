vex::brain Brain;

vex::motor  left_front(vex::PORT18, vex::gearSetting::ratio18_1, true);
vex::motor  left_back(vex::PORT17, vex::gearSetting::ratio18_1, true);
vex::motor  right_front(vex::PORT20, vex::gearSetting::ratio18_1, true);
vex::motor  right_back(vex::PORT19, vex::gearSetting::ratio18_1, true);
vex::motor  flywheel(vex::PORT2, vex::gearSetting::ratio18_1, true);
vex::motor  arm(vex::PORT3, vex::gearSetting::ratio18_1, true);
vex::motor  cap_intake(vex::PORT4, vex::gearSetting::ratio18_1, true);
vex::motor  ball_intake(vex::PORT1, vex::gearSetting::ratio18_1, true);

vex::controller controller(vex::controllerType::primary);

vex::competition  competition;

int flywheel_speed_percentage = 100;
int flywheel_rest_speed_threshold_percentage = 30;
int ball_intake_speed_percentage = 100;
int arm_speed_percentage = 70;

//Field
int tiles = 6;
int tile_px = 240/tiles;

int x_displacement = 120;
int y_displacement = 0;
int x = x_displacement;
int y = y_displacement;

int capsRadiusPX = 7;
int ballsRadiusPX = 1;