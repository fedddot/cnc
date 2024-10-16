package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

var s_test_gpo_map map[string]int = map[string]int{
	"a0": 10,
	"a1": 11,
	"b0": 12,
	"b1": 13,
	"en": 14,
}
var s_test_id string = "motor1"

func TestInitUninit(t *testing.T) {
	// GIVEN:
	connection := communication.TestConnection{}

	// WHEN
	connection.Init(
		func(request communication.Request) (communication.Response, error) {
			// THEN
			for gpo_field, gpo_id := range s_test_gpo_map {
				assert.NotEqual(t, nil, request.Body.(map[string]interface{})[gpo_field])
				assert.Equal(t, gpo_id, request.Body.(map[string]interface{})[gpo_field])
			}
			assert.Equal(t, s_test_id, request.Body.(map[string]interface{})["id"])
			return communication.Response{ResultCode: 200, Body: map[string]interface{}{}}, nil
		},
	)
	instance := ProxyStepperMotor{}
	err := instance.Init(
		s_test_gpo_map,
		s_test_id,
		&connection,
	)
	assert.Equal(t, nil, err, "received error is not nil")
}
