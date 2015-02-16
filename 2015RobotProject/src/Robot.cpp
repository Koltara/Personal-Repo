#include "WPILib.h"

/*
 * Preliminary Code for FRC Team 5053 - The Lakers for the 2015 Season "Recycle Rush"

-- Analog Channels --
1.
2. Arm Potentiometer (Preliminary)
3. Claw Potentiometer
4.
5.
6.

-- PWM Ports --
0. Left Drive 	- Talon
1. Right Drive 	- Talon
2. Lift Motor	- Talon
3. Claw Motor	- Talon
4.
5.
6.
7.
8.
9.
10.

-- Digital I/O Ports --
0. Left Drive Encoder
1. Right Drive Encoder
2. Elevator Encoder (Preliminary)
3.
4.
5.
6.
7.
8.





*/
enum AutonomousSelector {
	AutoDoNothing,
	AutoMoveForward,
	AutoPushTote,

};
#define BUTTON_X 1
#define BUTTON_A 2
#define BUTTON_B 3
#define BUTTON_Y 4
#define BUTTON_LB 5
#define BUTTON_RB 6
#define BUTTON_LT 7
#define BUTTON_RT 8
#define BUTTON_SEL 9
#define BUTTON_STA 10

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;

	RobotDrive	*m_robotDrive;
	Joystick 	*m_Driver;

	Talon		*m_Lift;
	Talon		*m_Claw;

	PIDController *m_ElevatorLiftPID;
	PIDController *m_ArmLiftPID;
	PIDController *m_ClawPID;

	AnalogInput	*m_ArmPot;
	AnalogInput	*m_ClawPot;
	Encoder 	*m_LeftDriveEncoder;
	Encoder 	*m_RightDriveEncoder;
	Encoder		*m_LiftEncoder;

	SmartDashboard *m_Dash;

	AutonomousSelector autoSelector;

	bool m_DiagnosticMode;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();

		m_Dash = new SmartDashboard();


		m_robotDrive = new RobotDrive(0,1);
		m_Driver = new Joystick(0);

		m_Lift	= new Talon(2);
		m_Claw	= new Talon(3);

		m_ArmPot = new AnalogInput(2);
		m_ClawPot = new AnalogInput(3);
		m_LeftDriveEncoder = new Encoder(0, 1, false);
		m_RightDriveEncoder = new Encoder(2, 3, true);
		m_LiftEncoder = new Encoder(4, 5, false);

		m_ArmLiftPID = new PIDController(0.0, 0.0, 0.0, m_ArmPot, m_Lift, .05);
		m_ElevatorLiftPID = new PIDController(0.0, 0.0, 0.0, m_LiftEncoder, m_Lift, .05);
		m_ClawPID = new PIDController(0.0, 0.0, 0.0, m_ClawPot, m_Claw, .05);

		m_DiagnosticMode = false;

	}
	void DisabledInit()
	{

	}
	void DisabledPeriodic()
	{
		UpdateDashboard();
		if (m_Driver->GetRawButton(BUTTON_RB) && (!m_Driver->GetRawButton(BUTTON_RB)))
		{
			if (autoSelector > AutoPushTote)
				autoSelector = AutoPushTote;
			else
				autoSelector++;
		} else if (m_Driver->GetRawButton(BUTTON_LB) && (!m_Driver->GetRawButton(BUTTON_LB)))
		{
			if (autoSelector < AutoDoNothing)
				autoSelector = AutoDoNothing;
			else
				autoSelector--;
		}
	}

	void AutonomousInit()
	{
		ResetControllers();
		ResetSensors();


	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{
		m_ArmLiftPID->Disable();
		m_ElevatorLiftPID->Disable();
		m_ClawPID->Disable();

		ResetControllers();
		ResetSensors();


	//	m_ArmLiftPID->Enable();
	//	m_ElevatorLiftPID->Enable();
	//	m_ClawPID->Enable();
	}

	void TeleopPeriodic()
	{

		if (!m_DiagnosticMode)
		{

			if (m_Driver->GetRawButton(9) && m_Driver->GetRawButton(10))
				m_DiagnosticMode = true;

			TeleopDrive();
			UpdateDashboard();
		} else
		{
			if (m_DiagnosticMode && (m_Driver->GetRawButton(11)))
				m_DiagnosticMode = false;

		}

	}

	void TestPeriodic()
	{
		lw->Run();

	}
	void UpdateDashboard(void)
	{
		// Update the Smart Dashboard on the Driver Station with the information

		m_Dash->PutNumber("Arm Voltage", m_ArmPot->GetValue());
		m_Dash->PutNumber("Claw Voltage", m_ClawPot->GetValue());
		m_Dash->PutNumber("Lift Ticks", m_LiftEncoder->GetDistance());
	}
	// Resets all PID Controllers
	void ResetControllers(void)
	{
		m_ElevatorLiftPID->Reset();
		m_ArmLiftPID->Reset();
		m_ClawPID->Reset();
	}
	// Resets all Sensors
	void ResetSensors(void)
	{
		m_LiftEncoder->Reset();
		m_LeftDriveEncoder->Reset();
		m_RightDriveEncoder->Reset();

	}
	void TeleopDrive(void)
	{

		// If Left Stick not centered or If Right stick is not centered
		if ((m_Driver->GetRawAxis(2) > 0.1 || m_Driver->GetRawAxis(2) < 0.1) || (m_Driver->GetRawAxis(4) > 0.1 || m_Driver->GetRawAxis(4) < 0.1))
					m_robotDrive(m_Driver->GetRawAxis(2), m_Driver->GetRawAxis(4));

	}
};

START_ROBOT_CLASS(Robot);
