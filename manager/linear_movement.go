package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type LinearMovementConfig struct {
	Target model.Vector[float32] `json:"target"`
	Feed   float32               `json:"feed"`
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

func (i *LinearMovement) Move(target model.Vector[float32], feed float32) error {
	request_body := LinearMovementConfig{
		Target: target,
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
