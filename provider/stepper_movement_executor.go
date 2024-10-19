package provider

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
)

type DimensionToMotorMapping map[model.Dimension]string

type MotorDirection int

const (
	CW  MotorDirection = 0
	CCW MotorDirection = 1
)

type StepsConfiguration struct {
	Direction   MotorDirection `json:"dir"`
	StepsNumber uint           `json:"steps_num"`
	OnTime      uint           `json:"on_time"`
	OffTime     uint           `json:"off_time"`
}

type MovementConfiguration struct {
	MotorId  string             `json:"motor_id"`
	StepsCfg StepsConfiguration `json:"steps_cfg"`
}

type MovementsConfiguration struct {
	Movements []MovementConfiguration `json:"movements"`
}

type MovementsBody struct {
	Id     string                 `json:"id"`
	Config MovementsConfiguration `json:"config"`
}

type StepperMovementExecutor struct {
	steps_per_mm         float32
	dim_to_motor_mapping DimensionToMotorMapping
	connection           communication.Connection
}

func (i *StepperMovementExecutor) Init(steps_per_mm float32, dim_to_motor_mapping DimensionToMotorMapping) error {
	for _, dim := range []model.Dimension{model.X, model.Y, model.Z} {
		_, ok := dim_to_motor_mapping[dim]
		if !ok {
			return fmt.Errorf("invalid mapping received - missing dimension %d", dim)
		}
	}
	i.steps_per_mm = steps_per_mm
	i.dim_to_motor_mapping = dim_to_motor_mapping
	return nil
}

func (i *StepperMovementExecutor) Execute(movement model.Vector, feed float32) error {
	err, body := i.generateMovements(movement, feed)
	if err != nil {
		return err
	}
	resp, err := i.connection.RunRequest(
		communication.Request{
			Route:  "movements",
			Method: "POST",
			Body:   body,
		},
	)
	if err != nil {
		return err
	}
	if resp.ResultCode != 0 {
		return fmt.Errorf("failed to execute: server returned a failure code")
	}
	return nil
}

func (i *StepperMovementExecutor) generateMovements(movement model.Vector, feed float32) (error, MovementsBody) {

}
