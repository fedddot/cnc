package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_Gpio_Init_Uninit(t *testing.T) {
	// GIVEN
	connection := communication.HttpConnection{}
	test_create_cfg := GpioCreateConfig{
		Id: "test_gpio",
		Config: GpioConfig{
			GpioId:    17,
			Direction: GpioDirection(IN),
		},
	}

	// WHEN
	connection.Init("http://127.0.0.1", "5000")
	instance := Gpio{}

	// THEN
	err := instance.Init(test_create_cfg, &connection)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
