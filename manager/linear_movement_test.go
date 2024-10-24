package manager

import (
	"cnc/client/communication"
	"cnc/client/model"
	"encoding/json"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMovement_Init_Move_Uninit(t *testing.T) {
	// GIVEN
	create_cfg := MovementCreateConfig{
		Id: "test_movement",
		Config: MovementConfig{
			MotorsMapping: MotorsMapping{
				model.X: "motor1",
				model.Y: "motor2",
				model.Z: "motor3",
			},
			Type: LINEAR,
		},
	}
	steps_per_unit := uint(100)
	time_divider := uint(1000000) // s -> us

	test_vector := model.Vector[float32]{
		X: 23.1,
		Y: 26.2,
		Z: 34.5,
	}
	test_feed := float32(16)

	// WHEN
	connection := communication.TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			switch request.Method {
			case "POST":
				parsed_body := json.Unmarshal(*)
			}
			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
		},
	)
	instance := LinearMovement{}

	// THEN
	err := instance.Init(create_cfg, &connection, steps_per_unit, time_divider)
	assert.Equal(t, nil, err)

	err = instance.Move(test_vector, test_feed)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
