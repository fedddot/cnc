package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type CircularMovementDirection uint

const (
	CW CircularMovementDirection = iota
	CCW
)

type CircularMovementConfig struct {
	Target         model.Vector[int]         `json:"target"`
	RotationCenter model.Vector[int]         `json:"rotation_center"`
	Direction      CircularMovementDirection `json:"direction"`
	Feed           uint                      `json:"feed"`
}

type CircularMovement struct {
	Movement
	steps_per_unit uint
	time_divider   uint
}

func (i *CircularMovement) Init(movement_config MovementCreateConfig, connection communication.Connection, steps_per_length_unit uint, time_divider uint) error {
	err := i.Movement.Init(movement_config, connection)
	if err != nil {
		return err
	}
	i.steps_per_unit = steps_per_length_unit
	i.time_divider = time_divider
	return nil
}

func (i CircularMovement) generateMovementVector(vector model.Vector[float32]) model.Vector[int] {
	return model.Vector[int]{
		X: int(float32(i.steps_per_unit) * vector.X),
		Y: int(float32(i.steps_per_unit) * vector.Y),
		Z: int(float32(i.steps_per_unit) * vector.Z),
	}
}

func (i *CircularMovement) Move(target, rotation_center model.Vector[float32], direction CircularMovementDirection, feed float32) error {
	steps_feed := uint(feed * float32(i.steps_per_unit))
	request_body := CircularMovementConfig{
		Target:         i.generateMovementVector(target),
		RotationCenter: i.generateMovementVector(rotation_center),
		Direction:      direction,
		Feed:           steps_feed,
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
