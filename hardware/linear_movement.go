package hardware

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type Dimension int

const (
	X Dimension = iota
	Y
	Z
)

type LinearMovementAxesConfig struct {
	Inverse int `json:"inverse"`
}

type LinearMovementCreateConfig struct {
	Id     ResourceId               `json:"id"`
	Config LinearMovementAxesConfig `json:"config"`
}

type LinearMovementSteps map[ResourceId]int

type StepDurationUnit uint

type LinearMovementDescription struct {
	Steps        []LinearMovementSteps `json:"steps"`
	StepDuration StepDurationUnit      `json:"step_duration"`
}

type LinearMovementMoveConfig struct {
	Config LinearMovementDescription `json:"config"`
}

type LinearMovement struct {
	config         LinearMovementCreateConfig
	steppers       map[Dimension]StepperMotor
	connection     communication.Connection
	steps_per_unit uint
	time_divider   uint
}

func (i *LinearMovement) Init(movement_config LinearMovementCreateConfig, steppers_mapping map[Dimension]StepperMotorCreateConfig, steps_per_length_unit uint, time_divider uint, connection communication.Connection) error {
	if connection == nil {
		return fmt.Errorf("invalid connection ptr received")
	}
	steppers := make(map[Dimension]StepperMotor, 0)
	for dim, cfg := range steppers_mapping {
		stepper := StepperMotor{}
		err := stepper.Init(cfg, connection)
		if err != nil {
			return err
		}
		steppers[dim] = stepper
	}
	request := communication.Request{
		Route:  "movements",
		Method: "POST",
		Body:   movement_config,
	}
	resp, err := connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	i.config = movement_config
	i.steppers = steppers
	i.connection = connection
	i.steps_per_unit = steps_per_length_unit
	i.time_divider = time_divider
	return nil
}

func (i *LinearMovement) Uninit() error {
	for _, motor := range i.steppers {
		err := motor.Uninit()
		if err != nil {
			return err
		}
	}
	request := communication.Request{
		Route:  fmt.Sprintf("movements/%s", i.config.Id),
		Method: "DELETE",
		Body:   map[string]interface{}{},
	}
	resp, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	i.steppers = make(map[Dimension]StepperMotor, 0)
	return nil
}

func (i LinearMovement) toStepsVector(vector model.Vector[float32]) model.Vector[int] {
	return model.Vector[int]{
		X: int(float32(i.steps_per_unit) * vector.X),
		Y: int(float32(i.steps_per_unit) * vector.Y),
		Z: int(float32(i.steps_per_unit) * vector.Z),
	}
}

func (i LinearMovement) toStepDuration(feed float32) (StepDurationUnit, error) {
	if feed <= 0 {
		return 0, fmt.Errorf("invalid feed received (must be greater than zero)")
	}
	steps_per_time_unit := uint(feed * float32(i.steps_per_unit)) // TODO: add correction: ... * float32(steps_to_perform) / float32(length_to_pass))
	if steps_per_time_unit == 0 {
		return 0, nil
	}
	return StepDurationUnit(i.time_divider / steps_per_time_unit), nil
}

func (i *LinearMovement) Move(movements []model.Vector[float32], feed float32) error {
	steps := make([]LinearMovementSteps, 0)
	for _, movement := range movements {
		steps_vector := i.toStepsVector(movement)
		steps_item := LinearMovementSteps{
			i.steppers[X].Id(): steps_vector.X,
			i.steppers[Y].Id(): steps_vector.Y,
			i.steppers[Z].Id(): steps_vector.Z,
		}
		steps = append(steps, steps_item)
	}
	step_duration, err := i.toStepDuration(feed)
	if err != nil {
		return err
	}
	request_body := LinearMovementMoveConfig{
		Config: LinearMovementDescription{
			Steps:        steps,
			StepDuration: step_duration,
		},
	}
	request := communication.Request{
		Route:  fmt.Sprintf("movements/%s", i.config.Id),
		Method: "PUT",
		Body:   request_body,
	}
	resp, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	return nil
}
