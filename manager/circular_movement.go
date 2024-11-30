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
	Target         model.Vector[model.FloatCoordinate] `json:"target"`
	RotationCenter model.Vector[model.FloatCoordinate] `json:"rotation_center"`
	Direction      CircularMovementDirection           `json:"direction"`
	Feed           float32                             `json:"feed"`
}

type CircularMovement struct {
	Movement
}

func (i *CircularMovement) Init(id model.ResourceId, motors_mapping MotorsMapping, steps_per_length uint, connection communication.Connection) error {
	return i.Movement.Init(
		id,
		MovementConfig{
			MotorsMapping:  motors_mapping,
			Type:           CIRCULAR_INTERPOLATION,
			StepsPerLength: steps_per_length,
		},
		connection,
	)
}

func (i *CircularMovement) Move(target model.Vector[model.FloatCoordinate], rotation_center model.Vector[model.FloatCoordinate], direction CircularMovementDirection, feed float32) error {
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
