package hardware

import (
	"cnc/client/communication"
	"cnc/client/model"
	"testing"

	"github.com/stretchr/testify/assert"
)

var s_x_motor_gpo_map = map[string]int{
	"a0": 16,
	"a1": 17,
	"b0": 18,
	"b1": 19,
	"en": 15,
}

var s_y_motor_gpo_map = map[string]int{
	"a0": 6,
	"a1": 5,
	"b0": 4,
	"b1": 3,
	"en": 7,
}

var s_z_motor_gpo_map = map[string]int{
	"a0": 13,
	"a1": 12,
	"b0": 11,
	"b1": 10,
	"en": 14,
}

var s_dimension_to_motor_mapping = map[model.Dimension]string{
	model.X: "x_motor",
	model.Y: "y_motor",
	model.Z: "z_motor",
}

func GenerateMovements(vector model.Vector, splits_number uint) ([]MovementConfiguration, error) {
	split_element_size := float32(5.0 / 500.0)
	on_time := uint(500)
	off_time := uint(100)

	result := make([]MovementConfiguration, 0)
	split_res, err := model.SplitVector(vector, splits_number, split_element_size)
	if err != nil {
		return result, nil
	}
	for dim, steps_num := range split_res.RepetativeMovement {
		dir := CW
		if steps_num < 0 {
			dir = CCW
			steps_num = -steps_num
		}
		movement_cfg := MovementConfiguration{
			MotorId: s_dimension_to_motor_mapping[dim],
			StepsCfg: StepsConfiguration{
				Direction:   dir,
				StepsNumber: uint(steps_num),
				OnTime:      on_time,
				OffTime:     off_time,
			},
		}
		result = append(result, movement_cfg)
	}
	return result, nil
}

func TestMovement(t *testing.T) {
	// GIVEN:
	sign := float32(-1.0)
	vector := model.Vector{
		model.X: -15 * sign,
		model.Y: +15 * sign,
		model.Z: 5.0 * sign,
	}

	// WHEN
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")
	motor_x := ProxyStepperMotor{}
	err := motor_x.Init(
		s_x_motor_gpo_map,
		"x_motor",
		&connection,
	)
	assert.Equal(t, nil, err, "received error is not nil")

	motor_y := ProxyStepperMotor{}
	err = motor_y.Init(
		s_y_motor_gpo_map,
		"y_motor",
		&connection,
	)
	assert.Equal(t, nil, err, "received error is not nil")

	motor_z := ProxyStepperMotor{}
	err = motor_z.Init(
		s_z_motor_gpo_map,
		"z_motor",
		&connection,
	)
	assert.Equal(t, nil, err, "received error is not nil")

	movements := Movements{}
	movements.Init(&connection)

	repeats := uint(500)

	a_movements, _ := GenerateMovements(vector, repeats)

	err = movements.Move(a_movements, repeats)
	assert.Equal(t, nil, err, "received error is not nil")

	motor_x.Uninit()
	motor_y.Uninit()
	motor_z.Uninit()
}
