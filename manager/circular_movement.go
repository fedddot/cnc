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
	Target         model.Vector[float32]     `json:"target"`
	RotationCenter model.Vector[float32]     `json:"rotation_center"`
	Direction      CircularMovementDirection `json:"direction"`
	Feed           float32                   `json:"feed"`
}

type CircularMovement struct {
	Movement
}

func (i *CircularMovement) Init(movement_config MovementCreateConfig, connection communication.Connection) error {
	err := i.Movement.Init(movement_config, connection)
	if err != nil {
		return err
	}
	return nil
}

func (i *CircularMovement) Move(target model.Vector[float32], rotation_center model.Vector[float32], direction CircularMovementDirection, feed float32) error {
	request_body := CircularMovementConfig{
		Target:         target,
		RotationCenter: rotation_center,
		Direction:      direction,
		Feed:           feed,
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
		return fmt.Errorf("server failure, code = %d, what = %v", resp.ResultCode, resp.Body)
	}
	return nil
}
