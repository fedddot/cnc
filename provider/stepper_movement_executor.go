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

func (i *StepperMovementExecutor) Init(steps_per_mm float32, dim_to_motor_mapping DimensionToMotorMapping, connection communication.Connection) error {
	for _, dim := range []model.Dimension{model.X, model.Y, model.Z} {
		_, ok := dim_to_motor_mapping[dim]
		if !ok {
			return fmt.Errorf("invalid mapping received - missing dimension %d", dim)
		}
	}
	if steps_per_mm == 0.0 {
		return fmt.Errorf("invalid steps per mm ratio received - can't be zero")
	}
	i.steps_per_mm = steps_per_mm
	i.dim_to_motor_mapping = dim_to_motor_mapping
	i.connection = connection
	return nil
}

func (i *StepperMovementExecutor) Execute(movement model.Vector, feed float32) error {
	body, err := i.generateMovements(movement, feed)
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

func (i *StepperMovementExecutor) generateMovements(movement model.Vector, feed float32) (MovementsBody, error) {
	// movements := make([]MovementConfiguration, 0)
	steps_numbers := i.vectorToStepsNumbers(movement)
	fmt.Printf("steps_numbers = %v\n", steps_numbers)
	return MovementsBody{}, fmt.Errorf("NOT IMPLEMENTED")
}

func (i *StepperMovementExecutor) vectorToStepsNumbers(vector model.Vector) map[string]int {
	steps_numbers := make(map[string]int, 0)
	for _, dim := range []model.Dimension{model.X, model.Y, model.Z} {
		steps_numbers[i.dim_to_motor_mapping[dim]] = int(i.steps_per_mm * vector.Get(dim))
	}
	return steps_numbers
}
