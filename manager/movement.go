package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type MovementType int

const (
	LINEAR MovementType = iota
	CIRCULAR_INTERPOLATION
)

type MotorsMapping map[model.Dimension]model.ResourceId

type MovementConfig struct {
	MotorsMapping string       `json:"motors_mapping"`
	Type          MovementType `json:"type"`
}

type MovementCreateConfig struct {
	Id     model.ResourceId `json:"id"`
	Config MovementConfig   `json:"config"`
}

type Movement struct {
	Config     MovementCreateConfig
	Connection communication.Connection
}

func (i *Movement) Init(movement_config MovementCreateConfig, connection communication.Connection) error {
	if connection == nil {
		return fmt.Errorf("invalid connection ptr received")
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
	i.Config = movement_config
	i.Connection = connection
	return nil
}

func (i *Movement) Uninit() error {
	request := communication.Request{
		Route:  fmt.Sprintf("movements/%s", i.Config.Id),
		Method: "DELETE",
		Body:   map[string]interface{}{},
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
