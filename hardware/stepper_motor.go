package hardware

import (
	"cnc/client/communication"
	"fmt"
)

type Direction int

const (
	CW  Direction = 0
	CCW Direction = 1
)

type ResourceId string

type StepperMotorGpoMapping struct {
	A0 ResourceId `json:"a0"`
	A1 ResourceId `json:"a1"`
	B0 ResourceId `json:"b0"`
	B1 ResourceId `json:"b1"`
	En ResourceId `json:"en"`
}

type StepperMotorCreateConfig struct {
	Id     ResourceId             `json:"id"`
	Config StepperMotorGpoMapping `json:"config"`
}

type StepperMotorUpdateConfig struct {
	Config map[string]interface{} `json:"config"`
}

type StepperMotor struct {
	create_config StepperMotorCreateConfig
	connection    communication.Connection
}

func (i *StepperMotor) Init(config StepperMotorCreateConfig, connection communication.Connection) error {
	i.create_config = config
	i.connection = connection

	request := communication.Request{
		Route:  "steppers",
		Method: "POST",
		Body:   i.create_config,
	}
	response, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if response.ResultCode != 0 {
		return fmt.Errorf("server failure, code: %d", response.ResultCode)
	}
	return nil
}

func (i *StepperMotor) Uninit() error {
	request := communication.Request{
		Route:  fmt.Sprintf("steppers/%s", i.create_config.Id),
		Method: "DELETE",
		Body:   map[string]interface{}{},
	}
	response, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if response.ResultCode != 0 {
		return fmt.Errorf("server failure, code: %d", response.ResultCode)
	}
	return nil
}
