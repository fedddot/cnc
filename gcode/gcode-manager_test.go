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
	// connection := communication.TestConnection{}
	// connection.Init(
	// 	func(request communication.Request) (communication.Response, error) {
	// 		return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
	// 	},
	// )
	steps_per_unit := uint(100)
	steppers_mapping := manager.MotorsMapping{
		"x": "motor1",
		"y": "motor2",
		"z": "motor3",
	}

	steppers := initSteppers(t, &connection)
	defer uninitSteppers(t, steppers)

	linear_movement := initLinearMovement(t, &connection, steps_per_unit, steppers_mapping)
	defer linear_movement.Uninit()
	circular_movement := initCircularMovement(t, &connection, steps_per_unit, steppers_mapping)
	defer circular_movement.Uninit()

	manager := GcodeManager{}
	err := manager.Init(linear_movement, circular_movement)
	assert.Equal(t, nil, err)

	commands, err := readCommandsFromFile("/usr/src/app/asdf.gcode")
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

func initLinearMovement(t *testing.T, connection communication.Connection, steps_per_unit uint, mapping manager.MotorsMapping) manager.LinearMovement {
	linear_movement_cfg := manager.MovementCreateConfig{
		Id: "linear",
		Config: manager.MovementConfig{
			Type:           manager.LINEAR,
			MotorsMapping:  mapping,
			StepsPerLength: steps_per_unit,
		},
	}
	linear_movement := manager.LinearMovement{}
	err := linear_movement.Init(linear_movement_cfg, connection)
	assert.Equal(t, nil, err)
	return linear_movement
}

func initCircularMovement(t *testing.T, connection communication.Connection, steps_per_unit uint, mapping manager.MotorsMapping) manager.CircularMovement {
	circular_movement_cfg := manager.MovementCreateConfig{
		Id: "circular",
		Config: manager.MovementConfig{
			MotorsMapping:  mapping,
			Type:           manager.CIRCULAR_INTERPOLATION,
			StepsPerLength: steps_per_unit,
		},
	}
	circular_movement := manager.CircularMovement{}
	err := circular_movement.Init(circular_movement_cfg, connection)
	assert.Equal(t, nil, err)
	return circular_movement
}
