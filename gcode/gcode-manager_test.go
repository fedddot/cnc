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
	linear_movement := initLinearMovement(t)
	defer linear_movement.Uninit()

	manager := GcodeManager{}
	err := manager.Init(&linear_movement)
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

func initLinearMovement(t *testing.T) manager.LinearMovement {
	// GIVEN
	// connection := communication.TestConnection{}
	// connection.Init(
	// 	func(request communication.Request) (communication.Response, error) {
	// 		fmt.Printf("Processing request: %v\n", request)
	// 		return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
	// 	},
	// )

	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")
	motor1 := hardware.StepperMotor{}
	motor1.Init(
		hardware.StepperMotorCreateConfig{
			Id: "motor1",
			Config: hardware.StepperMotorGpoMapping{
				A0: 16,
				A1: 17,
				B0: 18,
				B1: 19,
				En: 15,
			},
		},
		&connection,
	)
	motor2 := hardware.StepperMotor{}
	motor2.Init(
		hardware.StepperMotorCreateConfig{
			Id: "motor2",
			Config: hardware.StepperMotorGpoMapping{
				A0: 6,
				A1: 5,
				B0: 4,
				B1: 3,
				En: 7,
			},
		},
		&connection,
	)
	motor3 := hardware.StepperMotor{}
	motor3.Init(
		hardware.StepperMotorCreateConfig{
			Id: "motor3",
			Config: hardware.StepperMotorGpoMapping{
				A0: 13,
				A1: 12,
				B0: 11,
				B1: 10,
				En: 14,
			},
		},
		&connection,
	)
	create_cfg := manager.MovementCreateConfig{
		Id: "test_movement",
		Config: manager.MovementConfig{
			MotorsMapping: manager.MotorsMapping{
				"x": "motor1",
				"y": "motor2",
				"z": "motor3",
			},
			Type:           manager.LINEAR,
			TimeMultiplier: 1000000,
		},
	}
	steps_per_unit := uint(100)
	time_divider := uint(1000000)

	instance := manager.LinearMovement{}
	err := instance.Init(create_cfg, &connection, steps_per_unit, time_divider)
	assert.Equal(t, nil, err)
	return instance
}
