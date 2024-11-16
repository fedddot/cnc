package hardware

import (
	"cnc/client/communication"
	"encoding/json"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func run_test_request(request communication.Request) (communication.Response, error) {
	body, _ := json.Marshal(request.Body)
	fmt.Printf("\n\nrunning %s request on route %s with body:\n%s\n", request.Method, request.Route, body)
	return communication.Response{ResultCode: 200, Body: nil}, nil
}

func Test_Gpio_Init_Uninit(t *testing.T) {
	// GIVEN
	connection := communication.TestConnection{}
	test_create_cfg := GpioCreateConfig{
		Id: "test_gpio",
		Config: GpioConfig{
			GpioId:    17,
			Direction: GpioDirection(CW),
		},
	}

	// WHEN
	connection.Init(run_test_request)
	instance := Gpio{}

	// THEN
	err := instance.Init(test_create_cfg, &connection)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
