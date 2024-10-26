package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type LinearMovementConfig struct {
	MovementVector model.Vector[int] `json:"vector"`
	Feed           uint              `json:"feed"`
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

func (i *LinearMovement) Move(vector model.Vector[float32], feed float32) error {
	movement_vector := model.Vector[int]{
		X: int(float32(i.steps_per_unit) * vector.X),
		Y: int(float32(i.steps_per_unit) * vector.Y),
		Z: int(float32(i.steps_per_unit) * vector.Z),
	}
	steps_feed := uint(float32(feed) * float32(i.steps_per_unit))
	request_body := LinearMovementConfig{
		MovementVector: movement_vector,
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
