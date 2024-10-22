package hardware

import (
	"cnc/client/communication"
	"cnc/client/model"
	"math"
	"testing"

	"github.com/stretchr/testify/assert"
)

func createMovementBetweenSectors(phi1, phi2, radius float64) model.Vector[float32] {
	result := model.Vector[float32]{}
	result.Init(
		float32(radius*(math.Cos(phi2)-math.Cos(phi1))),
		float32(radius*(math.Sin(phi2)-math.Sin(phi1))),
		0,
	)
	return result
}

func createChordMovements(radius float32, number_of_sectors uint) []model.Vector[float32] {
	res := make([]model.Vector[float32], 0)
	for sector_index := uint(1); sector_index < number_of_sectors; sector_index++ {
		sector_movement := createMovementBetweenSectors(
			2*math.Pi*float64(sector_index-1)/float64(number_of_sectors),
			2*math.Pi*float64(sector_index)/float64(number_of_sectors),
			float64(radius),
		)
		res = append(res, sector_movement)
	}
	return res
}

func TestMovement_Init_Move_Uninit(t *testing.T) {
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
				A0: 16,
				A1: 17,
				B0: 18,
				B1: 19,
				En: 15,
			},
		},
		model.Y: {
			Id: "Y",
			Config: StepperMotorGpoMapping{
				A0: 6,
				A1: 5,
				B0: 4,
				B1: 3,
				En: 7,
			},
		},
		model.Z: {
			Id: "Z",
			Config: StepperMotorGpoMapping{
				A0: 13,
				A1: 12,
				B0: 11,
				B1: 10,
				En: 14,
			},
		},
	}
	steps_per_unit := uint(100)
	time_divider := uint(1000000) // s -> us

	test_feed := float32(16)

	// WHEN
	connection := communication.TestConnection{}
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
		},
	)
	instance := LinearMovement{}

	// THEN
	err := instance.Init(create_cfg, motors_mapping, steps_per_unit, time_divider, &connection)
	assert.Equal(t, nil, err)

	err = instance.Move(createChordMovements(10, 50), test_feed)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
