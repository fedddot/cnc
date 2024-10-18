package hardware

type Position float32

type Axis struct {
	tag              string
	motor            StepperMotor
	steps_per_mm     uint
	current_position Position
}

func (i *Axis) Init(tag string, motor StepperMotor, steps_per_mm uint) {
	i.tag = tag
	i.motor = motor
	i.steps_per_mm = steps_per_mm
	i.current_position = 0.0
}
func (i *Axis) ResetPosition(position Position) {
	i.current_position = position
}
func (i *Axis) GetPosition() Position {
	return i.current_position
}
func (i *Axis) MoveAbs(absolute_position Position) error {
	delta := absolute_position - i.current_position
	direction := CW
	if delta < 0.0 {
		direction = CCW
		delta = -delta
	}
	steps_number := uint(delta * Position(i.steps_per_mm))
	err := i.motor.Steps(steps_number, direction, 300, 100) // TODO: derive times from the feed speed
	if err != nil {
		return err
	}
	i.current_position = absolute_position
	return nil
}
func (i *Axis) MoveRel(relative_position Position) error {
	return i.MoveAbs(i.current_position + relative_position)
}
