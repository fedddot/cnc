package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMovement_Init_Move_Uninit(t *testing.T) {
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

	test_vector := model.Vector[float32]{
		X: 5.0,
		Y: 0,
		Z: 0,
	}
	test_feed := float32(16)

	// WHEN
	// connection := communication.TestConnection{}
	// connection.Init(
	// 	func(request communication.Request) (communication.Response, error) {
	// 		return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
	// 	},
	// )
	connection := communication.HttpConnection{}
	connection.Init("http://localhost", "5000")
	instance := LinearMovement{}

	// THEN
	err := instance.Init(create_cfg, &connection, steps_per_unit, time_divider)
	assert.Equal(t, nil, err)

	err = instance.Move(test_vector, test_feed)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
