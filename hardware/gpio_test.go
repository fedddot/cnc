package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_Gpio_Init_Uninit(t *testing.T) {
	// GIVEN
	connection := communication.HttpConnection{}
	test_id := string("test_gpio")
	test_num := GpioNumber(11)
	test_dir := OUT

	// WHEN
	connection.Init("http://127.0.0.1", "5000")
	instance := Gpio{}

	// THEN
	err := instance.Init(test_id, test_num, test_dir, &connection)
	assert.Equal(t, nil, err)

	err = instance.Uninit()
	assert.Equal(t, nil, err)
}
