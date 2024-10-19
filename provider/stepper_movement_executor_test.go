package provider

import (
	"cnc/client/communication"
	"cnc/client/model"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

type TestConnection struct {
	action func(request communication.Request) (communication.Response, error)
}

func (i *TestConnection) Init(action func(request communication.Request) (communication.Response, error)) {
	i.action = action
}

func (i *TestConnection) RunRequest(request communication.Request) (communication.Response, error) {
	return i.action(request)
}

func TestExecutor(t *testing.T) {
	// GIVEN
	test_steps_per_mm := float32(500.0)
	test_dim_to_motor_mapping := DimensionToMotorMapping{
		model.X: "motor_x",
		model.Y: "motor_y",
		model.Z: "motor_z",
	}
	test_movement := model.Vector{}
	test_movement.Init(10.8, 14.98, 39.1)
	test_feed := float32(394240.724)

	// WHEN
	connection := TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			return communication.Response{}, fmt.Errorf("NOT IMPLEMENTED")
		},
	)

	// THEN
	test_executor := StepperMovementExecutor{}
	err := test_executor.Init(test_steps_per_mm, test_dim_to_motor_mapping, &connection)
	assert.Equal(t, nil, err)

	err = test_executor.Execute(test_movement, test_feed)
	assert.Equal(t, nil, err)
}
