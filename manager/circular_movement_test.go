package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestCircularMovement_Init_Move_Uninit(t *testing.T) {
	// GIVEN
	movement_id := model.ResourceId("circular_movement")
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

	instance := CircularMovement{}

	// THEN
	err = instance.Init(
		movement_id,
		motors_mapping,
		steps_per_length,
		&connection,
	)
	assert.Equal(t, nil, err)
	defer instance.Uninit()

	err = instance.Move(
		model.Vector[model.FloatCoordinate]{X: 10, Y: 10, Z: 0},
		model.Vector[model.FloatCoordinate]{X: 10, Y: 0, Z: 0},
		CCW,
		3.0,
	)
	assert.Equal(t, nil, err)
}
