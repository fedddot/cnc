package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type LinearMovementConfig struct {
	Target model.Vector[model.FloatCoordinate] `json:"target"`
	Feed   float32                             `json:"feed"`
}

type LinearMovement struct {
	Movement
}

func (i *LinearMovement) Init(movement_config MovementCreateConfig, connection communication.Connection) error {
	err := i.Movement.Init(movement_config, connection)
	if err != nil {
		return err
	}
	return nil
}

func (i *LinearMovement) Move(target model.Vector[model.FloatCoordinate], feed float32) error {
	inverted_target := model.Vector[model.FloatCoordinate]{X: target.X, Y: -target.Y, Z: target.Z}
	request_body := LinearMovementConfig{
		Target: inverted_target,
		Feed:   feed,
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
