package hardware

type MotorDirection int

const (
	CW  MotorDirection = 0
	CCW MotorDirection = 1
)

type StepperMotor interface {
	Steps(steps_num uint, direction MotorDirection, time_on uint, time_off uint) error
}
