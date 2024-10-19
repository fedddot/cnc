package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

var s_test_gpo_map map[string]int = map[string]int{
	"a0": 16,
	"a1": 17,
	"b0": 18,
	"b1": 19,
	"en": 15,
}
var s_test_id string = "motor1"

// func TestInitUninit(t *testing.T) {
// 	// GIVEN:
// 	connection := communication.TestConnection{}

// 	// WHEN
// 	connection.Init(
// 		func(request communication.Request) (communication.Response, error) {
// 			// THEN
// 			for gpo_field, gpo_id := range s_test_gpo_map {
// 				assert.NotEqual(t, nil, request.Body.(map[string]interface{})[gpo_field])
// 				assert.Equal(t, gpo_id, request.Body.(map[string]interface{})[gpo_field])
// 			}
// 			assert.Equal(t, s_test_id, request.Body.(map[string]interface{})["id"])
// 			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
// 		},
// 	)
// 	instance := ProxyStepperMotor{}
// 	err := instance.Init(
// 		s_test_gpo_map,
// 		s_test_id,
// 		&connection,
// 	)
// 	assert.Equal(t, nil, err, "received error is not nil")
// }

// func TestInitUninitHttp(t *testing.T) {
// 	// GIVEN:
// 	connection := communication.HttpConnection{}

// 	// WHEN
// 	connection.Init("http://127.0.0.1", "5000")
// 	instance := ProxyStepperMotor{}
// 	err := instance.Init(
// 		s_test_gpo_map,
// 		s_test_id,
// 		&connection,
// 	)
// 	assert.Equal(t, nil, err, "received error is not nil")
// 	err = instance.Steps(1000, CCW, 500, 100)
// 	assert.Equal(t, nil, err, "received error is not nil")
// 	instance.Uninit()
// }

func TestComplexMovement(t *testing.T) {
	// GIVEN:
	x_motor_id := "x_motor"
	x_motor_mapping := map[string]int{
		"a0": 16,
		"a1": 17,
		"b0": 18,
		"b1": 19,
		"en": 15,
	}
	x_steps := map[string]int{
		"steps_num": 1,
		"dir":       0,
		"on_time":   500,
		"off_time":  100,
	}
	x_movement := map[string]interface{}{
		"motor_id":  x_motor_id,
		"steps_cfg": x_steps,
	}

	y_motor_id := "y_motor"
	y_motor_mapping := map[string]int{
		"a0": 6,
		"a1": 5,
		"b0": 4,
		"b1": 3,
		"en": 7,
	}
	y_steps := map[string]int{
		"steps_num": 2,
		"dir":       0,
		"on_time":   500,
		"off_time":  100,
	}
	y_movement := map[string]interface{}{
		"motor_id":  y_motor_id,
		"steps_cfg": y_steps,
	}

	repeats := 500

	movements := []interface{}{x_movement, y_movement}

	config := map[string]interface{}{
		"movements": movements,
		"repeats":   repeats,
	}

	body := map[string]interface{}{
		"id":     "blabla",
		"config": config,
	}

	// WHEN
	connection := communication.HttpConnection{}
	connection.Init("http://127.0.0.1", "5000")

	motor_x := ProxyStepperMotor{}
	err := motor_x.Init(
		x_motor_mapping,
		x_motor_id,
		&connection,
	)
	assert.Equal(t, nil, err, "received error is not nil")

	motor_y := ProxyStepperMotor{}
	err = motor_y.Init(
		y_motor_mapping,
		y_motor_id,
		&connection,
	)
	assert.Equal(t, nil, err, "received error is not nil")

	response, err := connection.RunRequest(
		communication.Request{
			Method: "POST",
			Route:  "movements",
			Body:   body,
		},
	)
	assert.Equal(t, nil, err, "received error is not nil")
	assert.Equal(t, 0, response.ResultCode, "received error is not nil")

	motor_x.Uninit()
	motor_y.Uninit()
}
