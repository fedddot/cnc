package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type LinearMovementSteps map[model.Dimension]int

type StepDurationUnit uint

type LinearMovementConfig struct {
	Steps        LinearMovementSteps `json:"steps"`
	StepDuration StepDurationUnit    `json:"step_duration"`
}

type LinearMovement struct {
	Movement
	steps_per_unit uint
	time_divider   uint
}

func (i *LinearMovement) Init(movement_config MovementCreateConfig, connection communication.Connection, steps_per_length_unit uint, time_divider uint) error {
	err := i.Movement.Init(movement_config, connection)
	if err != nil {
		return err
	}
	i.steps_per_unit = steps_per_length_unit
	i.time_divider = time_divider
	return nil
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

func (i *LinearMovement) Move(vector model.Vector[float32], feed float32) error {
	steps := LinearMovementSteps{
		model.X: int(float32(i.steps_per_unit) * vector.X),
		model.Y: int(float32(i.steps_per_unit) * vector.Y),
		model.Z: int(float32(i.steps_per_unit) * vector.Z),
	}
	step_duration, err := i.toStepDuration(feed)
	if err != nil {
		return err
	}
	request_body := LinearMovementConfig{
		Steps:        steps,
		StepDuration: step_duration,
	}
	request := communication.Request{
		Route:  fmt.Sprintf("movements/%s", i.Id),
		Method: "PUT",
		Body:   request_body,
	}
	resp, err := i.Connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	return nil
}
