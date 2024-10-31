package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestCircularMovement_Init_Move_Uninit(t *testing.T) {
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

	test_target := model.Vector[float32]{X: 10, Y: 10, Z: 0}
	test_rotation_center := model.Vector[float32]{X: 10, Y: 0, Z: 0}
	test_feed := float32(10)
	test_direction := CW

	// WHEN
	connection := communication.TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
		},
	)
	instance := CircularMovement{}
	motors, err := initMotors(create_cfg.Config.MotorsMapping, &connection)
	assert.Equal(t, nil, err)
	defer uninitMotors(motors)

	// THEN
	err = instance.Init(create_cfg, &connection, steps_per_unit, time_divider)
	assert.Equal(t, nil, err)

	err = instance.Move(test_target, test_rotation_center, test_direction, test_feed)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
