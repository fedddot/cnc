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
			Type:           CIRCULAR_INTERPOLATION,
			StepsPerLength: 100,
		},
	}

	target := model.Vector[model.FloatCoordinate]{
		X: -10,
		Y: -10,
		Z: 0,
	}
	center := model.Vector[model.FloatCoordinate]{
		X: -10,
		Y: 0,
		Z: 0,
	}
	dir := CW
	test_feed := float32(10)

	// WHEN
	// connection := communication.TestConnection{}
	// connection.Init(
	// 	func(request communication.Request) (communication.Response, error) {
	// 		return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
	// 	},
	// )
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")
	instance := CircularMovement{}
	motors, err := initMotors(create_cfg.Config.MotorsMapping, &connection)
	assert.Equal(t, nil, err)
	defer uninitMotors(motors)

	// THEN
	err = instance.Init(create_cfg, &connection)
	assert.Equal(t, nil, err)

	err = instance.Move(target, center, dir, test_feed)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
