package gcode

import (
	"bufio"
	"cnc/client/communication"
	"cnc/client/hardware"
	"cnc/client/manager"
	"fmt"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGcodeManager_RunCommand(t *testing.T) {
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")

	steppers := initSteppers(t, &connection)
	defer uninitSteppers(t, steppers)

	movements := initMovements(t, &connection)
	defer uninitMovements(t, movements)

	linear := movements[manager.LINEAR]
	circular := movements[manager.CIRCULAR_INTERPOLATION]

	manager := GcodeManager{}
	err := manager.Init(linear.(manager.LinearMovement), circular.(manager.CircularMovement))
	assert.Equal(t, nil, err)

	commands, err := readCommandsFromFile("/usr/src/app/front.ngc")
	assert.Equal(t, nil, err)

	for _, command := range commands {
		fmt.Printf("running gcode: %s\n", command)
		err = manager.RunCommand(command)
		assert.Equal(t, nil, err)
	}
	assert.Equal(t, nil, err)
}

func readCommandsFromFile(path string) ([]string, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	result := make([]string, 0)
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}
		result = append(result, line)
	}
	return result, nil
}

func initSteppers(t *testing.T, connection communication.Connection) []hardware.StepperMotor {
	configs := []hardware.StepperMotorCreateConfig{
		{
			Id: "motor1",
			Config: hardware.StepperMotorGpoMapping{
				A0: 16,
				A1: 17,
				B0: 18,
				B1: 19,
				En: 15,
			},
		},
		{
			Id: "motor2",
			Config: hardware.StepperMotorGpoMapping{
				A0: 6,
				A1: 5,
				B0: 4,
				B1: 3,
				En: 7,
			},
		},
		{
			Id: "motor3",
			Config: hardware.StepperMotorGpoMapping{
				A0: 13,
				A1: 12,
				B0: 11,
				B1: 10,
				En: 14,
			},
		},
	}
	result := make([]hardware.StepperMotor, 0)
	for _, cfg := range configs {
		stepper := hardware.StepperMotor{}
		err := stepper.Init(cfg, connection)
		assert.Equal(t, nil, err)
		result = append(result, stepper)
	}
	return result
}

func uninitSteppers(t *testing.T, steppers []hardware.StepperMotor) {
	for _, stepper := range steppers {
		err := stepper.Uninit()
		assert.Equal(t, nil, err)
	}
}

func initMovements(t *testing.T, connection communication.Connection) map[manager.MovementType]manager.Movement {
	result := make(map[manager.MovementType]manager.Movement, 0)
	steps_per_unit := uint(100)
	time_multiplier := uint(1000000)
	steppers_mapping := manager.MotorsMapping{
		"x": "motor1",
		"y": "motor2",
		"z": "motor3",
	}

	linear_movement_cfg := manager.MovementCreateConfig{
		Id: "linear_movement",
		Config: manager.MovementConfig{
			Type:           manager.LINEAR,
			MotorsMapping:  steppers_mapping,
			TimeMultiplier: time_multiplier,
		},
	}
	linear_movement := manager.LinearMovement{}
	err := linear_movement.Init(linear_movement_cfg, connection, steps_per_unit, time_multiplier)
	assert.Equal(t, nil, err)
	result[manager.LINEAR] = linear_movement.Movement

	circular_movement_cfg := manager.MovementCreateConfig{
		Id: "circular_movement",
		Config: manager.MovementConfig{
			MotorsMapping:  steppers_mapping,
			Type:           manager.CIRCULAR_INTERPOLATION,
			TimeMultiplier: time_multiplier,
		},
	}
	circular_movement := manager.CircularMovement{}
	err = circular_movement.Init(circular_movement_cfg, connection, steps_per_unit, time_multiplier)
	assert.Equal(t, nil, err)
	result[manager.CIRCULAR_INTERPOLATION] = circular_movement.Movement
	return result
}

func uninitMovements(t *testing.T, movements map[manager.MovementType]manager.Movement) {
	for _, movement := range movements {
		err := movement.Uninit()
		assert.Equal(t, nil, err)
	}
}
