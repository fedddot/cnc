package hardware

import (
	"cnc/client/communication"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestStepperMotor_Init_Uninit(t *testing.T) {
	// GIVEN
	connection := communication.TestConnection{}
	action := func(request communication.Request) (communication.Response, error) {
		return communication.Response{ResultCode: 0, Body: nil}, nil
	}
	create_config := StepperMotorCreateConfig{
		Id: "test_motor",
		Config: StepperMotorGpoMapping{
			A0: "10",
			A1: "20",
			B0: "30",
			B1: "40",
			En: "50",
		},
	}

	// WHEN
	connection.Init(action)
	motor := StepperMotor{}
	err := motor.Init(create_config, &connection)
	assert.Equal(t, nil, err)
	err = motor.Uninit()
	assert.Equal(t, nil, err)
}
