package hardware

import (
	"cnc/client/communication"
	"cnc/client/model"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestMovement_Init_Uninit(t *testing.T) {
	// GIVEN
	create_cfg := LinearMovementCreateConfig{
		Id: "test_movement",
		Config: LinearMovementAxesConfig{
			Inverse: 0,
		},
	}
	motors_mapping := map[model.Dimension]StepperMotorCreateConfig{
		model.X: {
			Id: "X",
			Config: StepperMotorGpoMapping{
				A0: "10",
				A1: "20",
				B0: "30",
				B1: "40",
				En: "50",
			},
		},
		model.Y: {
			Id: "Y",
			Config: StepperMotorGpoMapping{
				A0: "60",
				A1: "70",
				B0: "80",
				B1: "90",
				En: "100",
			},
		},
		model.Z: {
			Id: "Z",
			Config: StepperMotorGpoMapping{
				A0: "110",
				A1: "120",
				B0: "130",
				B1: "140",
				En: "150",
			},
		},
	}
	steps_per_unit := uint(100)
	time_divider := uint(1000000) // s -> us
	test_movement := model.Vector[float32]{}
	test_movement.Init(10.3, 12.43, 139.2)
	test_feed := float32(15.4)

	// WHEN
	connection := communication.TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			return communication.Response{}, nil
		},
	)
	instance := LinearMovement{}

	// THEN
	err := instance.Init(create_cfg, motors_mapping, steps_per_unit, time_divider, &connection)
	assert.Equal(t, nil, err)

	err = instance.Move(test_movement, test_feed)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
