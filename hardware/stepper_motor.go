package hardware

import (
	"cnc/client/communication"
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

	// TODO: init also the stepper
	return nil
}

func (i *StepperMotor) Uninit() error {
	// TODO: init also the stepper
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
