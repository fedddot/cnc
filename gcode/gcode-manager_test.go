package gcode

import (
	"bufio"
	"cnc/client/communication"
	"cnc/client/manager"
	"fmt"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGcodeManager_RunCommand(t *testing.T) {
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")
	steps_per_unit := uint(100)
	steppers_mapping := manager.MotorsMapping{
		manager.X: "motor1",
		manager.Y: "motor2",
		manager.Z: "motor3",
	}

	steppers, err := manager.InitMotors(steppers_mapping, &connection)
	assert.Equal(t, nil, err)
	defer manager.UninitMotors(steppers)

	linear_movement := initLinearMovement(t, &connection, steps_per_unit, steppers_mapping)
	defer linear_movement.Uninit()
	circular_movement := initCircularMovement(t, &connection, steps_per_unit, steppers_mapping)
	defer circular_movement.Uninit()

	manager := GcodeManager{}
	err = manager.Init(linear_movement, circular_movement)
	assert.Equal(t, nil, err)

	// commands, err := readCommandsFromFile("/usr/src/app/asdf.gcode")
	commands, err := readCommandsFromFile("/usr/src/app/holes.gcode")
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

func initLinearMovement(t *testing.T, connection communication.Connection, steps_per_unit uint, mapping manager.MotorsMapping) manager.LinearMovement {
	linear_movement := manager.LinearMovement{}
	err := linear_movement.Init("linear_movement", mapping, steps_per_unit, connection)
	assert.Equal(t, nil, err)
	return linear_movement
}

func initCircularMovement(t *testing.T, connection communication.Connection, steps_per_unit uint, mapping manager.MotorsMapping) manager.CircularMovement {
	circular_movement := manager.CircularMovement{}
	err := circular_movement.Init("circular_movement", mapping, steps_per_unit, connection)
	assert.Equal(t, nil, err)
	return circular_movement
}
