package manager

import (
	"cnc/client/communication"
	"cnc/client/hardware"
	"cnc/client/model"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func get_mappings(axis AxisTag) (hardware.StepperMotorControlOutputs, hardware.StepperMotorDirectionOutputs, error) {
	switch axis {
	case X:
		return hardware.StepperMotorControlOutputs{
				hardware.ENA: 14,
				hardware.ENB: 15,
			},
			hardware.StepperMotorDirectionOutputs{
				hardware.ATop: 10,
				hardware.ABtm: 11,
				hardware.BTop: 12,
				hardware.BBtm: 13,
			},
			nil
	case Y:
		return hardware.StepperMotorControlOutputs{
				hardware.ENA: 24,
				hardware.ENB: 25,
			},
			hardware.StepperMotorDirectionOutputs{
				hardware.ATop: 20,
				hardware.ABtm: 21,
				hardware.BTop: 22,
				hardware.BBtm: 23,
			},
			nil
	case Z:
		return hardware.StepperMotorControlOutputs{
				hardware.ENA: 34,
				hardware.ENB: 35,
			},
			hardware.StepperMotorDirectionOutputs{
				hardware.ATop: 30,
				hardware.ABtm: 31,
				hardware.BTop: 32,
				hardware.BBtm: 33,
			},
			nil
	}
	return nil, nil, fmt.Errorf("unsupported axis tag: %s", axis)
}

func initMotors(mapping MotorsMapping, connection communication.Connection) (map[AxisTag]hardware.StepperMotor, error) {
	result := make(map[AxisTag]hardware.StepperMotor, 0)
	for tag, id := range mapping {
		controls, directions, err := get_mappings(tag)
		if err != nil {
			return result, err
		}
		motor := hardware.StepperMotor{}
		err = motor.Init(
			hardware.StepperMotorId(id),
			hardware.StepperMotorConfig{
				ControlOutputs:   controls,
				DirectionOutputs: directions,
			},
			connection,
		)
		if err != nil {
			return result, err
		}
		result[tag] = motor
	}
	return result, nil
}

func uninitMotors(motors map[AxisTag]hardware.StepperMotor) error {
	for _, motor := range motors {
		err := motor.Uninit()
		if err != nil {
			return err
		}
	}
	return nil
}

func TestLinearMovement_Init_Move_Uninit(t *testing.T) {
	// GIVEN
	movement_id := model.ResourceId("linear_id")
	motors_mapping := MotorsMapping{
		X: "motor_x",
		Y: "motor_y",
		Z: "motor_z",
	}
	steps_per_length := uint(100)
	connection := communication.HttpConnection{}

	// WHEN
	connection.Init("http://127.0.0.1", "5000")
	motors, err := initMotors(motors_mapping, &connection)
	assert.Equal(t, nil, err)
	defer uninitMotors(motors)

	instance := LinearMovement{}

	// THEN
	err = instance.Init(
		movement_id,
		motors_mapping,
		steps_per_length,
		&connection,
	)
	assert.Equal(t, nil, err)
	defer instance.Uninit()
}
