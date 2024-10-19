package hardware

type StepperMotor interface {
	Steps(steps_num uint, direction MotorDirection, time_on uint, time_off uint) error
}
