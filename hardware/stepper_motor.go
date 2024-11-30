package hardware

import (
	"cnc/client/communication"
	"encoding/json"
	"fmt"
)

type StepperMotorId string

type StepperMotorDirection int

const (
	CW  StepperMotorDirection = 0
	CCW StepperMotorDirection = 1
)

type ControlOutput string

const (
	ENA ControlOutput = "ena"
	ENB ControlOutput = "enb"
)

type DirectionOutput string

const (
	ATop DirectionOutput = "a_top"
	ABtm DirectionOutput = "a_btm"
	BTop DirectionOutput = "b_top"
	BBtm DirectionOutput = "b_btm"
)

type StepperMotorControlOutputs map[ControlOutput]GpioNumber
type StepperMotorDirectionOutputs map[DirectionOutput]GpioNumber

type StepperMotorConfig struct {
	ControlOutputs   StepperMotorControlOutputs   `json:"control_outputs"`
	DirectionOutputs StepperMotorDirectionOutputs `json:"direction_outputs"`
}

type StepperMotor struct {
	id         StepperMotorId
	config     StepperMotorConfig
	connection communication.Connection
	delimeter  string
	gpos       []Gpio
}

func (i *StepperMotor) Init(id StepperMotorId, config StepperMotorConfig, connection communication.Connection) error {
	i.id = id
	i.config = config
	i.connection = connection
	i.delimeter = "-"
	i.gpos = make([]Gpio, 0)

	err := i.initGpos()
	if err != nil {
		return err
	}

	request_body := map[string]interface{}{
		"id":     i.id,
		"config": i.generateStepperConfig(),
	}
	request := communication.Request{
		Route:  "steppers",
		Method: "POST",
		Body:   request_body,
	}
	resp, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		response_body, _ := json.Marshal(resp.Body)
		return fmt.Errorf("server returned failure code: %d; %s", resp.ResultCode, response_body)
	}
	return nil
}

func (i StepperMotor) generateStepperConfig() interface{} {
	control_outputs := make(map[ControlOutput]interface{}, 0)
	for control_tag := range i.config.ControlOutputs {
		control_outputs[control_tag] = fmt.Sprintf("%s%s%s", i.id, i.delimeter, control_tag)
	}
	direction_outputs := make(map[DirectionOutput]interface{}, 0)
	for direction_tag := range i.config.DirectionOutputs {
		direction_outputs[direction_tag] = fmt.Sprintf("%s%s%s", i.id, i.delimeter, direction_tag)
	}
	states := []map[DirectionOutput]GpioState{
		{
			ATop: HIGH,
			ABtm: LOW,
			BTop: LOW,
			BBtm: LOW,
		},
	}
	return map[string]interface{}{
		"control_outputs":   control_outputs,
		"direction_outputs": direction_outputs,
		"states":            states,
	}
}

func (i *StepperMotor) Uninit() error {
	request_body := make(map[string]interface{}, 0)
	request := communication.Request{
		Route:  fmt.Sprintf("steppers/%s", i.id),
		Method: "DELETE",
		Body:   request_body,
	}
	resp, err := i.connection.RunRequest(request)
	if err != nil {
		return err
	}
	if resp.ResultCode != 200 {
		response_body, _ := json.Marshal(resp.Body)
		return fmt.Errorf("server returned failure code: %d; %s", resp.ResultCode, response_body)
	}
	return i.uninitGpos()
}

func (i *StepperMotor) initGpos() error {
	for control_output_id, control_output_gpo_number := range i.config.ControlOutputs {
		gpo := Gpio{}
		err := gpo.Init(
			fmt.Sprintf("%s%s%s", i.id, i.delimeter, control_output_id),
			control_output_gpo_number,
			OUT,
			i.connection,
		)
		if err != nil {
			return err
		}
		i.gpos = append(i.gpos, gpo)
	}
	for direction_output_id, direction_output_gpo_number := range i.config.DirectionOutputs {
		gpo := Gpio{}
		err := gpo.Init(
			fmt.Sprintf("%s%s%s", i.id, i.delimeter, direction_output_id),
			direction_output_gpo_number,
			OUT,
			i.connection,
		)
		if err != nil {
			return err
		}
		i.gpos = append(i.gpos, gpo)
	}
	return nil
}

func (i *StepperMotor) uninitGpos() error {
	for _, gpo := range i.gpos {
		gpo.Uninit()
	}
	i.gpos = make([]Gpio, 0)
	return nil
}
