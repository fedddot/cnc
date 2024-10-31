package manager

import (
	"cnc/client/communication"
	"cnc/client/hardware"
	"cnc/client/model"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func initMotors(mapping MotorsMapping, connection communication.Connection) (map[string]hardware.StepperMotor, error) {
	result := make(map[string]hardware.StepperMotor, 0)
	for tag, id := range mapping {
		var gpo_mapping hardware.StepperMotorGpoMapping
		switch tag {
		case "x":
			gpo_mapping = hardware.StepperMotorGpoMapping{
				A0: 16,
				A1: 17,
				B0: 18,
				B1: 19,
				En: 15,
			}
		case "y":
			gpo_mapping = hardware.StepperMotorGpoMapping{
				A0: 6,
				A1: 5,
				B0: 4,
				B1: 3,
				En: 7,
			}
		case "z":
			gpo_mapping = hardware.StepperMotorGpoMapping{
				A0: 13,
				A1: 12,
				B0: 11,
				B1: 10,
				En: 14,
			}
		default:
			return result, fmt.Errorf("invalid dimension tag received: %s", tag)
		}
		stepper_cfg := hardware.StepperMotorCreateConfig{
			Id:     id,
			Config: gpo_mapping,
		}
		stepper := hardware.StepperMotor{}
		err := stepper.Init(stepper_cfg, connection)
		if err != nil {
			return result, err
		}
		result[tag] = stepper
	}
	return result, nil
}

func uninitMotors(motors map[string]hardware.StepperMotor) error {
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
	create_cfg := MovementCreateConfig{
		Id: "test_movement",
		Config: MovementConfig{
			MotorsMapping: MotorsMapping{
				"x": "motor1",
				"y": "motor2",
				"z": "motor3",
			},
			Type:           LINEAR,
			TimeMultiplier: 1000000,
		},
	}
	steps_per_unit := uint(100)
	time_divider := uint(1000000) // s -> us

	test_vectors := []model.Vector[float32]{
		{
			X: 0.0,
			Y: 0.0,
			Z: 3.0,
		},
		{
			X: 0.0,
			Y: 0.0,
			Z: -6.0,
		},
		{
			X: 0.0,
			Y: 0.0,
			Z: 3.0,
		},
	}
	test_feed := float32(10)

	// WHEN
	connection := communication.TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
		},
	)
	instance := LinearMovement{}
	motors, err := initMotors(create_cfg.Config.MotorsMapping, &connection)
	assert.Equal(t, nil, err)
	defer uninitMotors(motors)

	// THEN
	err = instance.Init(create_cfg, &connection, steps_per_unit, time_divider)
	assert.Equal(t, nil, err)

	for _, test_vector := range test_vectors {
		err = instance.Move(test_vector, test_feed)
		assert.Equal(t, nil, err)
	}

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
