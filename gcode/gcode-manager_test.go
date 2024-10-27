package gcode

import (
	"cnc/client/communication"
	"cnc/client/manager"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGcodeManager_RunCommand(t *testing.T) {
	linear_movement := initLinearMovement(t)
	defer linear_movement.Uninit()

	manager := GcodeManager{}
	err := manager.Init(&linear_movement)
	assert.Equal(t, nil, err)

	err = manager.RunCommand("G90")
	assert.Equal(t, nil, err)
	err = manager.RunCommand("G01 X13.4 Y16.3 F399.0")
	assert.Equal(t, nil, err)
	err = manager.RunCommand("G01 X100.4 Y13.3 F399.0")
	assert.Equal(t, nil, err)
}

func initLinearMovement(t *testing.T) manager.LinearMovement {
	// GIVEN
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
	time_divider := uint(100)

	// WHEN
	connection := communication.TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			fmt.Printf("Processing request: %v\n", request)
			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
		},
	)
	instance := manager.LinearMovement{}
	err := instance.Init(create_cfg, &connection, steps_per_unit, time_divider)
	assert.Equal(t, nil, err)
	return instance
}
