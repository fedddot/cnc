package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestAxisInitUninitHttp(t *testing.T) {
	// GIVEN:
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")
	motor := ProxyStepperMotor{}
	err := motor.Init(
		s_test_gpo_map,
		s_test_id,
		&connection,
	)
	assert.Equal(t, nil, err)

	// WHEN
	instance := Axis{}
	instance.Init("X", &motor, 100)
	err = instance.MoveAbs(10.0)
	assert.Equal(t, nil, err)

	motor.Uninit()
}

func TestAxisInitUninitHttpComplexMovement(t *testing.T) {
	// GIVEN:
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")
	motor_x := ProxyStepperMotor{}
	err := motor_x.Init(
		s_test_gpo_map,
		s_test_id,
		&connection,
	)
	assert.Equal(t, nil, err)
	motor_y := ProxyStepperMotor{}
	err = motor_y.Init(
		map[string]int{
			"a0": 6,
			"a1": 5,
			"b0": 4,
			"b1": 3,
			"en": 7,
		},
		"motor2",
		&connection,
	)

	// WHEN
	x := Axis{}
	x.Init("X", &motor_x, 100)
	y := Axis{}
	y.Init("Y", &motor_y, 100)

	steps_num := 10
	dx := Position(0.1)
	dy := Position(0.2)

	for {
		err = x.MoveRel(dx)
		assert.Equal(t, nil, err)
		err = y.MoveRel(dy)
		assert.Equal(t, nil, err)
		steps_num--
		if steps_num < 0 {
			break
		}
	}

	motor_x.Uninit()
	motor_y.Uninit()
}
