package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"encoding/json"
	"fmt"
)

type AxisTag string

const (
	X AxisTag = "x"
	Y AxisTag = "y"
	Z AxisTag = "z"
)

type MovementType int

const (
	LINEAR MovementType = iota
	CIRCULAR_INTERPOLATION
)

type MotorsMapping map[AxisTag]model.ResourceId

type MovementConfig struct {
	MotorsMapping  MotorsMapping `json:"steppers"`
	Type           MovementType  `json:"type"`
	StepsPerLength uint          `json:"steps_per_length"`
}

type Movement struct {
	Id         model.ResourceId
	Config     MovementConfig
	Connection communication.Connection
}

func (i *Movement) Init(id model.ResourceId, config MovementConfig, connection communication.Connection) error {
	if connection == nil {
		return fmt.Errorf("invalid connection ptr received")
	}
	request_body := map[string]interface{}{
		"id":     id,
		"config": config,
	}
	request := communication.Request{
		Route:  "movements",
		Method: "POST",
		Body:   request_body,
	}
	resp, err := connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		response_body, _ := json.Marshal(resp.Body)
		return fmt.Errorf("server returned failure code: %d; %s", resp.ResultCode, response_body)
	}
	i.Id = id
	i.Config = config
	i.Connection = connection
	return nil
}

func (i *Movement) Uninit() error {
	request := communication.Request{
		Route:  fmt.Sprintf("movements/%s", i.Id),
		Method: "DELETE",
		Body:   map[string]interface{}{},
	}
	resp, err := i.Connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		response_body, _ := json.Marshal(resp.Body)
		return fmt.Errorf("server returned failure code: %d; %s", resp.ResultCode, response_body)
	}
	return nil
}
