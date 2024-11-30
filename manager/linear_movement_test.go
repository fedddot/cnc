package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestLinearMovement_Init_Move_Uninit(t *testing.T) {
	// GIVEN
	movement_id := model.ResourceId("linear_movement")
	motors_mapping := MotorsMapping{
		X: "motor_x",
		Y: "motor_y",
		Z: "motor_z",
	}
	steps_per_length := uint(100)
	connection := communication.HttpConnection{}

	// WHEN
	connection.Init("http://127.0.0.1", "5000")
	motors, err := InitMotors(motors_mapping, &connection)
	assert.Equal(t, nil, err)
	defer UninitMotors(motors)

	instance := LinearMovement{}

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
		model.Vector[model.FloatCoordinate]{X: -3.020, Y: -1.530, Z: 0},
		3.0,
	)
	assert.Equal(t, nil, err)
}
