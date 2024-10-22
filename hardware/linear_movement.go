package hardware

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
	"math"
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

type LinearMovementMoveConfig struct {
	Steps        LinearMovementSteps `json:"steps"`
	StepDuration StepDurationUnit    `json:"step_duration"`
}

type LinearMovement struct {
	config         LinearMovementCreateConfig
	steppers       map[model.Dimension]StepperMotor
	connection     communication.Connection
	steps_per_unit uint
	time_divider   uint
}

func (i *LinearMovement) Init(movement_config LinearMovementCreateConfig, steppers_mapping map[model.Dimension]StepperMotorCreateConfig, steps_per_length_unit uint, time_divider uint, connection communication.Connection) error {
	if connection == nil {
		return fmt.Errorf("invalid connection ptr received")
	}
	steppers := make(map[model.Dimension]StepperMotor, 0)
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
	if resp.ResultCode != 0 {
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
	if resp.ResultCode != 0 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	i.steppers = make(map[model.Dimension]StepperMotor, 0)
	return nil
}

func (i LinearMovement) toStepsVector(vector model.Vector[float32]) model.Vector[int] {
	res := model.Vector[int]{}
	int_coords := make(map[model.Dimension]int, 0)
	for _, dim := range []model.Dimension{model.X, model.Y, model.Z} {
		int_coords[dim] = int(float32(i.steps_per_unit) * vector.Get(dim))
	}
	res.Init(int_coords[model.X], int_coords[model.Y], int_coords[model.Z])
	return res
}

func getPrimaryLength(steps_vector model.Vector[int]) uint {
	res := uint(0)
	for _, dim := range []model.Dimension{model.X, model.Y, model.Z} {
		abs_coord := uint(math.Abs(float64(steps_vector.Get(dim))))
		if abs_coord > res {
			res = abs_coord
		}
	}
	return res
}

func (i LinearMovement) toStepDuration(steps_vector model.Vector[int], feed float32) (StepDurationUnit, error) {
	if feed <= 0 {
		return 0, fmt.Errorf("invalid feed received (must be greater than zero)")
	}
	norm := steps_vector.Norm()
	if norm == 0 {
		return 0, nil
	}
	primary_length := getPrimaryLength(steps_vector)
	steps_per_second := uint(feed * float32(i.steps_per_unit) * float32(primary_length) / float32(norm))
	return StepDurationUnit(i.time_divider / steps_per_second), nil
}

func (i *LinearMovement) Move(movement model.Vector[float32], feed float32) error {
	steps := make(LinearMovementSteps, 0)
	steps_vector := i.toStepsVector(movement)
	for _, dim := range []model.Dimension{model.X, model.Y, model.Z} {
		steps[i.steppers[dim].Id()] = steps_vector.Get(dim)
	}
	step_duration, err := i.toStepDuration(steps_vector, feed)
	if err != nil {
		return err
	}
	request_body := LinearMovementMoveConfig{
		Steps:        steps,
		StepDuration: step_duration,
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
	if resp.ResultCode != 0 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	return nil
}
