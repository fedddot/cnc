package hardware

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type LinearMovementAxesConfig struct {
	Inverse int `json:"inverse"`
}

type LinearMovementCreateConfig struct {
	Id     ResourceId               `json:"id"`
	Config LinearMovementAxesConfig `json:"config"`
}

type Movement struct {
	config     LinearMovementCreateConfig
	steppers   map[model.Dimension]StepperMotor
	connection communication.Connection
}

func (i *Movement) Init(movement_config LinearMovementCreateConfig, steppers_mapping map[model.Dimension]StepperMotorCreateConfig, connection communication.Connection) error {
	if connection == nil {
		return fmt.Errorf("invalid connection ptr received")
	}
	steppers := make(map[model.Dimension]StepperMotor, 0)
	for dim, cfg := range steppers_mapping {
		stepper := StepperMotor{}
		err := stepper.Init(cfg, connection)
		if err != nil {
			return err
		}
		steppers[dim] = stepper
	}
	request := communication.Request{
		Route:  "movements",
		Method: "POST",
		Body:   movement_config,
	}
	resp, err := connection.RunRequest(request)
	if err == nil {
		return err
	}
	if resp.ResultCode != 0 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	i.config = movement_config
	i.steppers = steppers
	i.connection = connection
	return nil
}

func (i *Movement) Uninit() error {
	for _, motor := range i.steppers {
		err := motor.Uninit()
		if err != nil {
			return err
		}
	}
	request := communication.Request{
		Route:  fmt.Sprintf("movements/%s", i.config.Id),
		Method: "DELETE",
		Body:   map[string]interface{}{},
	}
	resp, err := i.connection.RunRequest(request)
	if err == nil {
		return err
	}
	if resp.ResultCode != 0 {
		return fmt.Errorf("server failure, code = %d", resp.ResultCode)
	}
	i.steppers = make(map[model.Dimension]StepperMotor, 0)
	return nil
}
